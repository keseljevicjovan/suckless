/* Copyright 2011-2020 Bert Muennich
 * Copyright 2021-2023 nsxiv contributors
 *
 * This file is a part of nsxiv.
 *
 * nsxiv is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * nsxiv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with nsxiv.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nsxiv.h"
#define INCLUDE_THUMBS_CONFIG
#include "config.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

#if HAVE_LIBEXIF
#include <libexif/exif-data.h>
#endif

static char *cache_dir;
static char *cache_tmpfile, *cache_tmpfile_base;
static const char TMP_NAME[] = "/nsxiv-XXXXXX";

static char *tns_cache_filepath(const char *filepath)
{
	size_t len;
	char *cfile = NULL;

	assert(*filepath == '/' && "filepath must be result of realpath(3)");

	if (strncmp(filepath, cache_dir, strlen(cache_dir)) != 0) {
		/* don't cache images inside the cache directory! */
		len = strlen(cache_dir) + strlen(filepath) + 2;
		cfile = emalloc(len);
		snprintf(cfile, len, "%s/%s", cache_dir, filepath + 1);
	}
	return cfile;
}

static Imlib_Image tns_cache_load(const char *filepath, bool *outdated)
{
	char *cfile;
	struct stat cstats, fstats;
	Imlib_Image im = NULL;

	if (stat(filepath, &fstats) < 0)
		return NULL;

	if ((cfile = tns_cache_filepath(filepath)) != NULL) {
		if (stat(cfile, &cstats) == 0) {
			if (cstats.st_mtime == fstats.st_mtime)
				im = imlib_load_image(cfile);
			else
				*outdated = true;
		}
		free(cfile);
	}
	return im;
}

static bool tns_cache_whitelisted(tns_t *tns, const char *filepath)
{
	ptrdiff_t i, dir_len = strrchr(filepath, '/') - filepath;
	dir_len = MAX(dir_len, 1); /* account for "/file" */

	if (tns->filters_cnt == 0)
		return true; /* no cache list, cache everything */

	for (i = 0; i < tns->filters_cnt; ++i) {
		thumb_filter_t *f = tns->filters + i;
		if ((f->recursive ? (dir_len >= f->len) : (dir_len == f->len)) &&
		     memcmp(filepath, f->path, f->len) == 0)
		{ /* in blacklist mode, finding a match means we shouldn't cache */
			return tns->filters_is_blacklist ? false : true;
		}
	}
	return tns->filters_is_blacklist; /* no match */
}

static void tns_cache_write(tns_t *tns, Imlib_Image im, const char *filepath, bool force)
{
	char *cfile, *dirend;
	int tmpfd;
	struct stat cstats, fstats;
	struct utimbuf times;
	Imlib_Load_Error err;

	if (options->private_mode || !tns_cache_whitelisted(tns, filepath))
		return;

	if (stat(filepath, &fstats) < 0)
		return;

	if ((cfile = tns_cache_filepath(filepath)) != NULL) {
		if (force || stat(cfile, &cstats) < 0 ||
		    cstats.st_mtime != fstats.st_mtime)
		{
			if ((dirend = strrchr(cfile, '/')) != NULL) {
				*dirend = '\0';
				if (r_mkdir(cfile) < 0)
					goto end;
				*dirend = '/';
			}
			imlib_context_set_image(im);
			if (imlib_image_has_alpha()) {
				imlib_image_set_format("png");
				imlib_image_attach_data_value("compression", NULL, 8, NULL);
			} else {
				imlib_image_set_format("jpg");
				imlib_image_attach_data_value("quality", NULL, 90, NULL);
			}
			memcpy(cache_tmpfile_base, TMP_NAME, sizeof(TMP_NAME));
			if ((tmpfd = mkstemp(cache_tmpfile)) < 0)
				goto end;
			close(tmpfd);
			/* UPGRADE: Imlib2 v1.11.0: use imlib_save_image_fd() */
			imlib_save_image_with_error_return(cache_tmpfile, &err);
			times.actime = fstats.st_atime;
			times.modtime = fstats.st_mtime;
			utime(cache_tmpfile, &times);
			if (err || rename(cache_tmpfile, cfile) < 0)
				unlink(cache_tmpfile);
		}
end:
		free(cfile);
	}
}

void tns_clean_cache(void)
{
	int dirlen;
	char *cfile, *filename;
	r_dir_t dir;

	if (r_opendir(&dir, cache_dir, true) < 0) {
		error(0, errno, "%s", cache_dir);
		return;
	}

	dirlen = strlen(cache_dir);

	while ((cfile = r_readdir(&dir, false)) != NULL) {
		filename = cfile + dirlen;
		if (access(filename, F_OK) < 0) {
			if (unlink(cfile) < 0)
				error(0, errno, "%s", cfile);
		}
		free(cfile);
	}
	r_closedir(&dir);
}

void tns_init(tns_t *tns, fileinfo_t *tns_files, const int *cnt, int *sel, win_t *win)
{
	int len;
	const char *homedir, *dsuffix = "";

	if (cnt != NULL && *cnt > 0)
		tns->thumbs = ecalloc(*cnt, sizeof(*tns->thumbs));
	else
		tns->thumbs = NULL;
	tns->files = tns_files;
	tns->cnt = cnt;
	tns->initnext = tns->loadnext = 0;
	tns->first = tns->end = tns->r_first = tns->r_end = 0;
	tns->sel = sel;
	tns->win = win;
	tns->dirty = false;

	tns->zl = THUMB_SIZE;
	tns_zoom(tns, 0);

	tns->filters = NULL;
	tns->filters_cnt = 0;
	tns->filters_is_blacklist = options->tns_filters_is_blacklist;
	if (options->tns_filters != NULL && options->tns_filters[0] != '\0') {
		int allocated = 0;
		char *save, *tok, *s = estrdup(options->tns_filters);
		for (tok = strtok_r(s, ":", &save); tok != NULL;
		     tok = strtok_r(NULL, ":", &save))
		{
			thumb_filter_t *f;
			if (tns->filters_cnt == allocated) {
				allocated = allocated > 0 ? (allocated * 2) : 4;
				tns->filters = erealloc(tns->filters,
				                        allocated * sizeof(*tns->filters));
			}
			f = tns->filters + tns->filters_cnt++;
			f->recursive = *tok == '*';
			f->path = realpath(tok + f->recursive, NULL);
			if (f->path == NULL) {
				error(EXIT_FAILURE, errno, "--cache-%s: `%s`",
				      tns->filters_is_blacklist ? "deny" : "allow",
				      tok + f->recursive);
			}
			f->len = strlen(f->path);
		}
		free(s);
	}

	if ((homedir = getenv("XDG_CACHE_HOME")) == NULL || homedir[0] == '\0') {
		homedir = getenv("HOME");
		dsuffix = "/.cache";
	}
	if (homedir != NULL) {
		const char *s = "/nsxiv";
		free(cache_dir);
		len = strlen(homedir) + strlen(dsuffix) + strlen(s) + 1;
		cache_dir = emalloc(len);
		snprintf(cache_dir, len, "%s%s%s", homedir, dsuffix, s);
		cache_tmpfile = emalloc(len + sizeof(TMP_NAME));
		memcpy(cache_tmpfile, cache_dir, len - 1);
		cache_tmpfile_base = cache_tmpfile + len - 1;
	} else {
		error(EXIT_FAILURE, 0, "Cache directory not found");
	}
}

CLEANUP void tns_free(tns_t *tns)
{
	int i;

	if (tns->thumbs != NULL) {
		for (i = 0; i < *tns->cnt; i++)
			img_free(tns->thumbs[i].im, false);
		free(tns->thumbs);
		tns->thumbs = NULL;
	}

	for (i = 0; i < tns->filters_cnt; ++i)
		free((void *)tns->filters[i].path);
	tns->filters_cnt = 0;
	free(tns->filters);
	tns->filters = NULL;

	free(cache_dir);
	cache_dir = NULL;
	free(cache_tmpfile);
	cache_tmpfile = cache_tmpfile_base = NULL;
}

static Imlib_Image tns_scale_down(Imlib_Image im, int dim)
{
	int w, h;
	float z, zw, zh;

	imlib_context_set_image(im);
	w = imlib_image_get_width();
	h = imlib_image_get_height();
	zw = (float)dim / (float)w;
	zh = (float)dim / (float)h;
	z = MIN(zw, zh);
	z = MIN(z, 1.0);

	if (z < 1.0) {
		imlib_context_set_anti_alias(1);
		im = imlib_create_cropped_scaled_image(0, 0, w, h,
		                                       MAX(z * w, 1), MAX(z * h, 1));
		if (im == NULL)
			error(EXIT_FAILURE, ENOMEM, NULL);
		imlib_free_image_and_decache();
	}
	return im;
}

bool tns_load(tns_t *tns, int n, bool force, bool cache_only)
{
	int maxwh = thumb_sizes[ARRLEN(thumb_sizes) - 1];
	bool cache_hit = false;
	char *cfile;
	thumb_t *t;
	fileinfo_t *file;
	Imlib_Image im = NULL;

	if (n < 0 || n >= *tns->cnt)
		return false;
	file = &tns->files[n];
	if (file->name == NULL || file->path == NULL)
		return false;

	t = &tns->thumbs[n];
	img_free(t->im, false);
	t->im = NULL;

	if (!force) {
		if ((im = tns_cache_load(file->path, &force)) != NULL) {
			imlib_context_set_image(im);
			if (imlib_image_get_width() < maxwh &&
			    imlib_image_get_height() < maxwh)
			{
				if ((cfile = tns_cache_filepath(file->path)) != NULL) {
					unlink(cfile);
					free(cfile);
				}
				imlib_free_image_and_decache();
				im = NULL;
			} else {
				cache_hit = true;
			}
#if HAVE_LIBEXIF
		} else if (!force && !options->private_mode) {
			int pw = 0, ph = 0, w, h, x = 0, y = 0;
			bool err;
			float zw, zh;
			ExifData *ed;
			ExifEntry *entry;
			ExifContent *ifd;
			ExifByteOrder byte_order;
			int tmpfd;
			char tmppath[] = "/tmp/nsxiv-XXXXXX";
			Imlib_Image tmpim;

			/* UPGRADE: Imlib2 v1.10.0: avoid tempfile and use imlib_load_image_mem() */
			if ((ed = exif_data_new_from_file(file->path)) != NULL) {
				if (ed->data != NULL && ed->size > 0 &&
				    (tmpfd = mkstemp(tmppath)) >= 0)
				{
					err = write(tmpfd, ed->data, ed->size) != ed->size;
					close(tmpfd);

					if (!err && (tmpim = imlib_load_image(tmppath)) != NULL) {
						byte_order = exif_data_get_byte_order(ed);
						ifd = ed->ifd[EXIF_IFD_EXIF];
						entry = exif_content_get_entry(ifd, EXIF_TAG_PIXEL_X_DIMENSION);
						if (entry != NULL)
							pw = exif_get_long(entry->data, byte_order);
						entry = exif_content_get_entry(ifd, EXIF_TAG_PIXEL_Y_DIMENSION);
						if (entry != NULL)
							ph = exif_get_long(entry->data, byte_order);

						imlib_context_set_image(tmpim);
						w = imlib_image_get_width();
						h = imlib_image_get_height();

						if (pw > w && ph > h && (pw - ph >= 0) == (w - h >= 0)) {
							zw = (float)pw / (float)w;
							zh = (float)ph / (float)h;
							if (zw < zh) {
								pw /= zh;
								x = (w - pw) / 2;
								w = pw;
							} else if (zw > zh) {
								ph /= zw;
								y = (h - ph) / 2;
								h = ph;
							}
						}
						if (w >= maxwh || h >= maxwh) {
							if ((im = imlib_create_cropped_image(x, y, w, h)) == NULL)
								error(0, 0, "%s: error generating thumbnail", file->name);
						}
						imlib_free_image_and_decache();
					}
					unlink(tmppath);
				}
				exif_data_unref(ed);
			}
#endif /* HAVE_LIBEXIF */
		}
	}

	if (im == NULL) {
		if ((im = img_open(file)) == NULL)
			return false;
	}
	imlib_context_set_image(im);

	if (!cache_hit) {
#if HAVE_LIBEXIF
		exif_auto_orientate(file);
#endif
		im = tns_scale_down(im, maxwh);
		imlib_context_set_image(im);
		if (imlib_image_get_width() == maxwh || imlib_image_get_height() == maxwh)
			tns_cache_write(tns, im, file->path, true);
	}

	if (cache_only) {
		imlib_free_image_and_decache();
	} else {
		t->im = tns_scale_down(im, thumb_sizes[tns->zl]);
		imlib_context_set_image(t->im);
		t->w = imlib_image_get_width();
		t->h = imlib_image_get_height();
		tns->dirty = true;
	}
	file->flags |= FF_TN_INIT;

	if (n == tns->initnext) {
		while (++tns->initnext < *tns->cnt && ((++file)->flags & FF_TN_INIT))
			;
	}
	if (n == tns->loadnext && !cache_only) {
		while (++tns->loadnext < tns->end && (++t)->im != NULL)
			;
	}

	return true;
}

void tns_unload(tns_t *tns, int n)
{
	thumb_t *t;

	assert(n >= 0 && n < *tns->cnt);
	t = &tns->thumbs[n];

	img_free(t->im, false);
	t->im = NULL;
}

static void tns_check_view(tns_t *tns, bool scrolled)
{
	int r;

	assert(tns != NULL);
	tns->first -= tns->first % tns->cols;
	r = *tns->sel % tns->cols;

	if (scrolled) {
		/* move selection into visible area */
		if (*tns->sel >= tns->first + tns->cols * tns->rows)
			*tns->sel = tns->first + r + tns->cols * (tns->rows - 1);
		else if (*tns->sel < tns->first)
			*tns->sel = tns->first + r;
	} else {
		/* scroll to selection */
		if (tns->first + tns->cols * tns->rows <= *tns->sel) {
			tns->first = *tns->sel - r - tns->cols * (tns->rows - 1);
			tns->dirty = true;
		} else if (tns->first > *tns->sel) {
			tns->first = *tns->sel - r;
			tns->dirty = true;
		}
	}
}

void tns_render(tns_t *tns)
{
	thumb_t *t;
	win_t *win;
	int i, cnt, r, x, y;

	if (!tns->dirty)
		return;

	win = tns->win;
	win_clear(win);
	imlib_context_set_drawable(win->buf.pm);

	tns->cols = MAX(1, win->w / tns->dim);
	tns->rows = MAX(1, win->h / tns->dim);

	if (*tns->cnt < tns->cols * tns->rows) {
		tns->first = 0;
		cnt = *tns->cnt;
	} else {
		tns_check_view(tns, false);
		cnt = tns->cols * tns->rows;
		if ((r = tns->first + cnt - *tns->cnt) >= tns->cols)
			tns->first -= r - r % tns->cols;
		if (r > 0)
			cnt -= r % tns->cols;
	}
	r = cnt % tns->cols ? 1 : 0;
	tns->x = x = (win->w - MIN(cnt, tns->cols) * tns->dim) / 2 + tns->bw + 3;
	tns->y = y = (win->h - (cnt / tns->cols + r) * tns->dim) / 2 + tns->bw + 3 +
	             (win->bar.top ? win->bar.h : 0);
	tns->loadnext = *tns->cnt;
	tns->end = tns->first + cnt;

	for (i = tns->r_first; i < tns->r_end; i++) {
		if ((i < tns->first || i >= tns->end) && tns->thumbs[i].im != NULL)
			tns_unload(tns, i);
	}
	tns->r_first = tns->first;
	tns->r_end = tns->end;

	for (i = tns->first; i < tns->end; i++) {
		t = &tns->thumbs[i];
		if (t->im != NULL) {
			t->x = x + (thumb_sizes[tns->zl] - t->w) / 2;
			t->y = y + (thumb_sizes[tns->zl] - t->h) / 2;
			imlib_context_set_image(t->im);
			render_core(win, 0, 0, t->w, t->h, t->x, t->y, t->w, t->h, false);
			if (tns->files[i].flags & FF_MARK)
				tns_mark(tns, i, true);
		} else {
			tns->loadnext = MIN(tns->loadnext, i);
		}
		if ((i + 1) % tns->cols == 0) {
			x = tns->x;
			y += tns->dim;
		} else {
			x += tns->dim;
		}
	}
	tns->dirty = false;
	tns_highlight(tns, *tns->sel, true);
}

void tns_mark(tns_t *tns, int n, bool mark)
{
	if (n >= 0 && n < *tns->cnt && tns->thumbs[n].im != NULL) {
		win_t *win = tns->win;
		thumb_t *t = &tns->thumbs[n];
		unsigned long col = win->win_bg.pixel;
		int x = t->x + t->w, y = t->y + t->h;

		win_draw_rect(win, x - 1, y + 1, 1, tns->bw, true, 1, col);
		win_draw_rect(win, x + 1, y - 1, tns->bw, 1, true, 1, col);

		if (mark)
			col = win->mrk_fg.pixel;

		win_draw_rect(win, x, y, tns->bw + 2, tns->bw + 2, true, 1, col);

		if (!mark && n == *tns->sel)
			tns_highlight(tns, n, true);
	}
}

void tns_highlight(tns_t *tns, int n, bool hl)
{
	if (n >= 0 && n < *tns->cnt && tns->thumbs[n].im != NULL) {
		win_t *win = tns->win;
		thumb_t *t = &tns->thumbs[n];
		unsigned long col = hl ? win->win_fg.pixel : win->win_bg.pixel;
		int oxy = (tns->bw + 1) / 2 + 1, owh = tns->bw + 2;

		win_draw_rect(win, t->x - oxy, t->y - oxy, t->w + owh, t->h + owh,
		              false, tns->bw, col);

		if (tns->files[n].flags & FF_MARK)
			tns_mark(tns, n, true);
	}
}

bool tns_move_selection(tns_t *tns, direction_t dir, int cnt)
{
	int old, max;

	old = *tns->sel;
	cnt = cnt > 1 ? cnt : 1;

	switch (dir) {
	case DIR_UP:
		*tns->sel = MAX(*tns->sel - cnt * tns->cols, *tns->sel % tns->cols);
		break;
	case DIR_DOWN:
		max = tns->cols * ((*tns->cnt - 1) / tns->cols) +
		      MIN((*tns->cnt - 1) % tns->cols, *tns->sel % tns->cols);
		*tns->sel = MIN(*tns->sel + cnt * tns->cols, max);
		break;
	case DIR_LEFT:
		*tns->sel = MAX(*tns->sel - cnt, 0);
		break;
	case DIR_RIGHT:
		*tns->sel = MIN(*tns->sel + cnt, *tns->cnt - 1);
		break;
	}

	if (*tns->sel != old) {
		tns_highlight(tns, old, false);
		tns_check_view(tns, false);
		if (!tns->dirty)
			tns_highlight(tns, *tns->sel, true);
	}
	return *tns->sel != old;
}

bool tns_scroll(tns_t *tns, direction_t dir, bool screen)
{
	int d, max, old;

	old = tns->first;
	d = tns->cols * (screen ? tns->rows : 1);

	if (dir == DIR_DOWN) {
		max = *tns->cnt - tns->cols * tns->rows;
		if (*tns->cnt % tns->cols != 0)
			max += tns->cols - *tns->cnt % tns->cols;
		tns->first = MIN(tns->first + d, max);
	} else if (dir == DIR_UP) {
		tns->first = MAX(tns->first - d, 0);
	}

	if (tns->first != old) {
		tns_check_view(tns, true);
		tns->dirty = true;
	}
	return tns->first != old;
}

bool tns_zoom(tns_t *tns, int d)
{
	int i, oldzl;

	oldzl = tns->zl;
	tns->zl += -(d < 0) + (d > 0);
	tns->zl = MAX(tns->zl, 0);
	tns->zl = MIN(tns->zl, (int)ARRLEN(thumb_sizes) - 1);

	tns->bw = ((thumb_sizes[tns->zl] - 1) >> 5) + 1;
	tns->bw = MIN(tns->bw, 4);
	tns->dim = thumb_sizes[tns->zl] + 2 * tns->bw + 6;

	if (tns->zl != oldzl) {
		for (i = 0; i < *tns->cnt; i++)
			tns_unload(tns, i);
		tns->dirty = true;
	}
	return tns->zl != oldzl;
}

int tns_translate(tns_t *tns, int x, int y)
{
	int n;
	int x_max = tns->x + tns->dim * tns->cols;
	int y_max = tns->y + tns->dim * tns->rows;

	if (x < tns->x || y < tns->y || x > x_max || y > y_max)
		return -1;

	n = tns->first + (y - tns->y) / tns->dim * tns->cols +
	    (x - tns->x) / tns->dim;
	if (n >= *tns->cnt)
		n = -1;

	return n;
}
