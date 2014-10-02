/*
 * GStreamer
 * Copyright (C) 2008-2010 Sebastian Dröge <slomo@collabora.co.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "gstdeinterlacemethod.h"

G_DEFINE_ABSTRACT_TYPE (GstDeinterlaceMethod, gst_deinterlace_method,
    GST_TYPE_OBJECT);

gboolean
gst_deinterlace_method_supported (GType type, GstVideoFormat format, gint width,
    gint height)
{
  GstDeinterlaceMethodClass *klass =
      GST_DEINTERLACE_METHOD_CLASS (g_type_class_ref (type));
  gboolean ret;

  if (format == GST_VIDEO_FORMAT_UNKNOWN)
    ret = TRUE;
  else
    ret = klass->supported (klass, format, width, height);
  g_type_class_unref (klass);

  return ret;
}

static gboolean
gst_deinterlace_method_supported_impl (GstDeinterlaceMethodClass * klass,
    GstVideoFormat format, gint width, gint height)
{
  switch (format) {
    case GST_VIDEO_FORMAT_YUY2:
      return (klass->deinterlace_frame_yuy2 != NULL);
    case GST_VIDEO_FORMAT_YVYU:
      return (klass->deinterlace_frame_yvyu != NULL);
    case GST_VIDEO_FORMAT_UYVY:
      return (klass->deinterlace_frame_uyvy != NULL);
    case GST_VIDEO_FORMAT_I420:
      return (klass->deinterlace_frame_i420 != NULL);
    case GST_VIDEO_FORMAT_YV12:
      return (klass->deinterlace_frame_yv12 != NULL);
    case GST_VIDEO_FORMAT_Y444:
      return (klass->deinterlace_frame_y444 != NULL);
    case GST_VIDEO_FORMAT_Y42B:
      return (klass->deinterlace_frame_y42b != NULL);
    case GST_VIDEO_FORMAT_Y41B:
      return (klass->deinterlace_frame_y41b != NULL);
    case GST_VIDEO_FORMAT_AYUV:
      return (klass->deinterlace_frame_ayuv != NULL);
    case GST_VIDEO_FORMAT_NV12:
      return (klass->deinterlace_frame_nv12 != NULL);
    case GST_VIDEO_FORMAT_NV21:
      return (klass->deinterlace_frame_nv21 != NULL);
    case GST_VIDEO_FORMAT_ARGB:
    case GST_VIDEO_FORMAT_xRGB:
      return (klass->deinterlace_frame_argb != NULL);
    case GST_VIDEO_FORMAT_ABGR:
    case GST_VIDEO_FORMAT_xBGR:
      return (klass->deinterlace_frame_abgr != NULL);
    case GST_VIDEO_FORMAT_RGBA:
    case GST_VIDEO_FORMAT_RGBx:
      return (klass->deinterlace_frame_rgba != NULL);
    case GST_VIDEO_FORMAT_BGRA:
    case GST_VIDEO_FORMAT_BGRx:
      return (klass->deinterlace_frame_bgra != NULL);
    case GST_VIDEO_FORMAT_RGB:
      return (klass->deinterlace_frame_rgb != NULL);
    case GST_VIDEO_FORMAT_BGR:
      return (klass->deinterlace_frame_bgr != NULL);
    default:
      return FALSE;
  }
}

void
gst_deinterlace_method_setup (GstDeinterlaceMethod * self, GstVideoInfo * vinfo)
{
  GstDeinterlaceMethodClass *klass = GST_DEINTERLACE_METHOD_GET_CLASS (self);

  klass->setup (self, vinfo);
}

static void
gst_deinterlace_method_setup_impl (GstDeinterlaceMethod * self,
    GstVideoInfo * vinfo)
{
  gint i;
  GstDeinterlaceMethodClass *klass = GST_DEINTERLACE_METHOD_GET_CLASS (self);

  self->vinfo = vinfo;

  self->deinterlace_frame = NULL;

  if (GST_VIDEO_INFO_FORMAT (self->vinfo) == GST_VIDEO_FORMAT_UNKNOWN)
    return;

  for (i = 0; i < 4; i++) {
    self->width[i] = GST_VIDEO_INFO_COMP_WIDTH (vinfo, i);
    self->height[i] = GST_VIDEO_INFO_COMP_HEIGHT (vinfo, i);
    self->offset[i] = GST_VIDEO_INFO_COMP_OFFSET (vinfo, i);
    self->row_stride[i] = GST_VIDEO_INFO_COMP_STRIDE (vinfo, i);
    self->pixel_stride[i] = GST_VIDEO_INFO_COMP_PSTRIDE (vinfo, i);
  }

  switch (GST_VIDEO_INFO_FORMAT (self->vinfo)) {
    case GST_VIDEO_FORMAT_YUY2:
      self->deinterlace_frame = klass->deinterlace_frame_yuy2;
      break;
    case GST_VIDEO_FORMAT_YVYU:
      self->deinterlace_frame = klass->deinterlace_frame_yvyu;
      break;
    case GST_VIDEO_FORMAT_UYVY:
      self->deinterlace_frame = klass->deinterlace_frame_uyvy;
      break;
    case GST_VIDEO_FORMAT_I420:
      self->deinterlace_frame = klass->deinterlace_frame_i420;
      break;
    case GST_VIDEO_FORMAT_YV12:
      self->deinterlace_frame = klass->deinterlace_frame_yv12;
      break;
    case GST_VIDEO_FORMAT_Y444:
      self->deinterlace_frame = klass->deinterlace_frame_y444;
      break;
    case GST_VIDEO_FORMAT_Y42B:
      self->deinterlace_frame = klass->deinterlace_frame_y42b;
      break;
    case GST_VIDEO_FORMAT_Y41B:
      self->deinterlace_frame = klass->deinterlace_frame_y41b;
      break;
    case GST_VIDEO_FORMAT_AYUV:
      self->deinterlace_frame = klass->deinterlace_frame_ayuv;
      break;
    case GST_VIDEO_FORMAT_NV12:
      self->deinterlace_frame = klass->deinterlace_frame_nv12;
      break;
    case GST_VIDEO_FORMAT_NV21:
      self->deinterlace_frame = klass->deinterlace_frame_nv21;
      break;
    case GST_VIDEO_FORMAT_ARGB:
    case GST_VIDEO_FORMAT_xRGB:
      self->deinterlace_frame = klass->deinterlace_frame_argb;
      break;
    case GST_VIDEO_FORMAT_ABGR:
    case GST_VIDEO_FORMAT_xBGR:
      self->deinterlace_frame = klass->deinterlace_frame_abgr;
      break;
    case GST_VIDEO_FORMAT_RGBA:
    case GST_VIDEO_FORMAT_RGBx:
      self->deinterlace_frame = klass->deinterlace_frame_rgba;
      break;
    case GST_VIDEO_FORMAT_BGRA:
    case GST_VIDEO_FORMAT_BGRx:
      self->deinterlace_frame = klass->deinterlace_frame_bgra;
      break;
    case GST_VIDEO_FORMAT_RGB:
      self->deinterlace_frame = klass->deinterlace_frame_rgb;
      break;
    case GST_VIDEO_FORMAT_BGR:
      self->deinterlace_frame = klass->deinterlace_frame_bgr;
      break;
    default:
      self->deinterlace_frame = NULL;
      break;
  }
}

static void
gst_deinterlace_method_class_init (GstDeinterlaceMethodClass * klass)
{
  klass->setup = gst_deinterlace_method_setup_impl;
  klass->supported = gst_deinterlace_method_supported_impl;
}

static void
gst_deinterlace_method_init (GstDeinterlaceMethod * self)
{
  self->vinfo = NULL;
}

void
gst_deinterlace_method_deinterlace_frame (GstDeinterlaceMethod * self,
    const GstDeinterlaceField * history, guint history_count,
    GstVideoFrame * outframe, int cur_field_idx)
{
  g_assert (self->deinterlace_frame != NULL);
  self->deinterlace_frame (self, history, history_count, outframe,
      cur_field_idx);
}

gint
gst_deinterlace_method_get_fields_required (GstDeinterlaceMethod * self)
{
  GstDeinterlaceMethodClass *klass = GST_DEINTERLACE_METHOD_GET_CLASS (self);

  return klass->fields_required;
}

gint
gst_deinterlace_method_get_latency (GstDeinterlaceMethod * self)
{
  GstDeinterlaceMethodClass *klass = GST_DEINTERLACE_METHOD_GET_CLASS (self);

  return klass->latency;
}

G_DEFINE_ABSTRACT_TYPE (GstDeinterlaceSimpleMethod,
    gst_deinterlace_simple_method, GST_TYPE_DEINTERLACE_METHOD);

static gboolean
gst_deinterlace_simple_method_supported (GstDeinterlaceMethodClass * mklass,
    GstVideoFormat format, gint width, gint height)
{
  GstDeinterlaceSimpleMethodClass *klass =
      GST_DEINTERLACE_SIMPLE_METHOD_CLASS (mklass);

  if (!GST_DEINTERLACE_METHOD_CLASS
      (gst_deinterlace_simple_method_parent_class)->supported (mklass, format,
          width, height))
    return FALSE;

  switch (format) {
    case GST_VIDEO_FORMAT_ARGB:
    case GST_VIDEO_FORMAT_xRGB:
      return (klass->interpolate_scanline_argb != NULL
          && klass->copy_scanline_argb != NULL);
    case GST_VIDEO_FORMAT_RGBA:
    case GST_VIDEO_FORMAT_RGBx:
      return (klass->interpolate_scanline_rgba != NULL
          && klass->copy_scanline_rgba != NULL);
    case GST_VIDEO_FORMAT_ABGR:
    case GST_VIDEO_FORMAT_xBGR:
      return (klass->interpolate_scanline_abgr != NULL
          && klass->copy_scanline_abgr != NULL);
    case GST_VIDEO_FORMAT_BGRA:
    case GST_VIDEO_FORMAT_BGRx:
      return (klass->interpolate_scanline_bgra != NULL
          && klass->copy_scanline_bgra != NULL);
    case GST_VIDEO_FORMAT_RGB:
      return (klass->interpolate_scanline_rgb != NULL
          && klass->copy_scanline_rgb != NULL);
    case GST_VIDEO_FORMAT_BGR:
      return (klass->interpolate_scanline_bgr != NULL
          && klass->copy_scanline_bgr != NULL);
    case GST_VIDEO_FORMAT_YUY2:
      return (klass->interpolate_scanline_yuy2 != NULL
          && klass->copy_scanline_yuy2 != NULL);
    case GST_VIDEO_FORMAT_YVYU:
      return (klass->interpolate_scanline_yvyu != NULL
          && klass->copy_scanline_yvyu != NULL);
    case GST_VIDEO_FORMAT_UYVY:
      return (klass->interpolate_scanline_uyvy != NULL
          && klass->copy_scanline_uyvy != NULL);
    case GST_VIDEO_FORMAT_AYUV:
      return (klass->interpolate_scanline_ayuv != NULL
          && klass->copy_scanline_ayuv != NULL);
    case GST_VIDEO_FORMAT_NV12:
      return (klass->interpolate_scanline_nv12 != NULL
          && klass->copy_scanline_nv12 != NULL);
    case GST_VIDEO_FORMAT_NV21:
      return (klass->interpolate_scanline_nv21 != NULL
          && klass->copy_scanline_nv21 != NULL);
    case GST_VIDEO_FORMAT_I420:
    case GST_VIDEO_FORMAT_YV12:
    case GST_VIDEO_FORMAT_Y444:
    case GST_VIDEO_FORMAT_Y42B:
    case GST_VIDEO_FORMAT_Y41B:
      return (klass->interpolate_scanline_planar_y != NULL
          && klass->copy_scanline_planar_y != NULL &&
          klass->interpolate_scanline_planar_u != NULL
          && klass->copy_scanline_planar_u != NULL &&
          klass->interpolate_scanline_planar_v != NULL
          && klass->copy_scanline_planar_v != NULL);
    default:
      return FALSE;
  }
}

static void
    gst_deinterlace_simple_method_interpolate_scanline_packed
    (GstDeinterlaceSimpleMethod * self, guint8 * out,
    const GstDeinterlaceScanlineData * scanlines)
{
  memcpy (out, scanlines->m1, self->parent.row_stride[0]);
}

static void
gst_deinterlace_simple_method_copy_scanline_packed (GstDeinterlaceSimpleMethod *
    self, guint8 * out, const GstDeinterlaceScanlineData * scanlines)
{
  memcpy (out, scanlines->m0, self->parent.row_stride[0]);
}

static void
gst_deinterlace_simple_method_deinterlace_frame_packed (GstDeinterlaceMethod *
    method, const GstDeinterlaceField * history, guint history_count,
    GstVideoFrame * outframe, gint cur_field_idx)
{
  GstDeinterlaceSimpleMethod *self = GST_DEINTERLACE_SIMPLE_METHOD (method);
  GstDeinterlaceMethodClass *dm_class = GST_DEINTERLACE_METHOD_GET_CLASS (self);
  GstDeinterlaceScanlineData scanlines;
  guint8 *dest;
  const guint8 *field0, *field1, *field2, *fieldp;
  guint cur_field_flags = history[cur_field_idx].flags;
  gint i;
  gint frame_height = GST_VIDEO_INFO_HEIGHT (self->parent.vinfo);
  gint stride = self->parent.row_stride[0];

  g_assert (self->interpolate_scanline_packed != NULL);
  g_assert (self->copy_scanline_packed != NULL);

  if (cur_field_idx > 0) {
    fieldp = GST_VIDEO_FRAME_COMP_DATA (history[cur_field_idx - 1].frame, 0);
  } else {
    fieldp = NULL;
  }

  dest = GST_VIDEO_FRAME_COMP_DATA (outframe, 0);

  field0 = GST_VIDEO_FRAME_COMP_DATA (history[cur_field_idx].frame, 0);

  g_assert (dm_class->fields_required <= 4);

  if (cur_field_idx + 1 < history_count) {
    field1 = GST_VIDEO_FRAME_COMP_DATA (history[cur_field_idx + 1].frame, 0);
  } else {
    field1 = NULL;
  }

  if (cur_field_idx + 2 < history_count) {
    field2 = GST_VIDEO_FRAME_COMP_DATA (history[cur_field_idx + 2].frame, 0);
  } else {
    field2 = NULL;
  }

#define CLAMP_LOW(i) (((i)<0) ? (i+2) : (i))
#define CLAMP_HI(i) (((i)>=(frame_height)) ? (i-2) : (i))
#define LINE(x,i) ((x) + CLAMP_HI(CLAMP_LOW(i)) * (stride))
#define LINE2(x,i) ((x) ? LINE(x,i) : NULL)

  for (i = 0; i < frame_height; i++) {
    memset (&scanlines, 0, sizeof (scanlines));
    scanlines.bottom_field = (cur_field_flags == PICTURE_INTERLACED_BOTTOM);

    if (!((i & 1) ^ scanlines.bottom_field)) {
      /* copying */
      scanlines.tp = LINE2 (fieldp, i - 1);
      scanlines.bp = LINE2 (fieldp, i + 1);

      scanlines.tt0 = LINE2 (field0, (i - 2 >= 0) ? i - 2 : i);
      scanlines.m0 = LINE2 (field0, i);
      scanlines.bb0 = LINE2 (field0, (i + 2 < frame_height ? i + 2 : i));

      scanlines.t1 = LINE2 (field1, i - 1);
      scanlines.b1 = LINE2 (field1, i + 1);

      scanlines.tt2 = LINE2 (field2, (i - 2 >= 0) ? i - 2 : i);
      scanlines.m2 = LINE2 (field2, i);
      scanlines.bb2 = LINE2 (field2, (i + 2 < frame_height ? i + 2 : i));

      self->copy_scanline_packed (self, LINE (dest, i), &scanlines);
    } else {
      /* interpolating */
      scanlines.ttp = LINE2 (fieldp, (i - 2 >= 0) ? i - 2 : i);
      scanlines.mp = LINE2 (fieldp, i);
      scanlines.bbp = LINE2 (fieldp, (i + 2 < frame_height ? i + 2 : i));

      scanlines.t0 = LINE2 (field0, i - 1);
      scanlines.b0 = LINE2 (field0, i + 1);

      scanlines.tt1 = LINE2 (field1, (i - 2 >= 0) ? i - 2 : i);
      scanlines.m1 = LINE2 (field1, i);
      scanlines.bb1 = LINE2 (field1, (i + 2 < frame_height ? i + 2 : i));

      scanlines.t2 = LINE2 (field2, i - 1);
      scanlines.b2 = LINE2 (field2, i + 1);

      self->interpolate_scanline_packed (self, LINE (dest, i), &scanlines);
    }
  }
}

static void
    gst_deinterlace_simple_method_interpolate_scanline_planar_y
    (GstDeinterlaceSimpleMethod * self, guint8 * out,
    const GstDeinterlaceScanlineData * scanlines)
{
  memcpy (out, scanlines->m1, self->parent.row_stride[0]);
}

static void
gst_deinterlace_simple_method_copy_scanline_planar_y (GstDeinterlaceSimpleMethod
    * self, guint8 * out, const GstDeinterlaceScanlineData * scanlines)
{
  memcpy (out, scanlines->m0, self->parent.row_stride[0]);
}

static void
    gst_deinterlace_simple_method_interpolate_scanline_planar_u
    (GstDeinterlaceSimpleMethod * self, guint8 * out,
    const GstDeinterlaceScanlineData * scanlines)
{
  memcpy (out, scanlines->m1, self->parent.row_stride[1]);
}

static void
gst_deinterlace_simple_method_copy_scanline_planar_u (GstDeinterlaceSimpleMethod
    * self, guint8 * out, const GstDeinterlaceScanlineData * scanlines)
{
  memcpy (out, scanlines->m0, self->parent.row_stride[1]);
}

static void
    gst_deinterlace_simple_method_interpolate_scanline_planar_v
    (GstDeinterlaceSimpleMethod * self, guint8 * out,
    const GstDeinterlaceScanlineData * scanlines)
{
  memcpy (out, scanlines->m1, self->parent.row_stride[2]);
}

static void
gst_deinterlace_simple_method_copy_scanline_planar_v (GstDeinterlaceSimpleMethod
    * self, guint8 * out, const GstDeinterlaceScanlineData * scanlines)
{
  memcpy (out, scanlines->m0, self->parent.row_stride[2]);
}

static void
    gst_deinterlace_simple_method_deinterlace_frame_planar_plane
    (GstDeinterlaceSimpleMethod * self, guint8 * dest, const guint8 * field0,
    const guint8 * field1, const guint8 * field2, const guint8 * fieldp,
    guint cur_field_flags,
    gint plane, GstDeinterlaceSimpleMethodFunction copy_scanline,
    GstDeinterlaceSimpleMethodFunction interpolate_scanline)
{
  GstDeinterlaceScanlineData scanlines;
  gint i;
  gint frame_height = self->parent.height[plane];
  gint stride = self->parent.row_stride[plane];

  g_assert (interpolate_scanline != NULL);
  g_assert (copy_scanline != NULL);

  for (i = 0; i < frame_height; i++) {
    memset (&scanlines, 0, sizeof (scanlines));
    scanlines.bottom_field = (cur_field_flags == PICTURE_INTERLACED_BOTTOM);

    if (!((i & 1) ^ scanlines.bottom_field)) {
      /* copying */
      scanlines.tp = LINE2 (fieldp, i - 1);
      scanlines.bp = LINE2 (fieldp, i + 1);

      scanlines.tt0 = LINE2 (field0, (i - 2 >= 0) ? i - 2 : i);
      scanlines.m0 = LINE2 (field0, i);
      scanlines.bb0 = LINE2 (field0, (i + 2 < frame_height ? i + 2 : i));

      scanlines.t1 = LINE2 (field1, i - 1);
      scanlines.b1 = LINE2 (field1, i + 1);

      scanlines.tt2 = LINE2 (field2, (i - 2 >= 0) ? i - 2 : i);
      scanlines.m2 = LINE2 (field2, i);
      scanlines.bb2 = LINE2 (field2, (i + 2 < frame_height ? i + 2 : i));

      copy_scanline (self, LINE (dest, i), &scanlines);
    } else {
      /* interpolating */
      scanlines.ttp = LINE2 (fieldp, (i - 2 >= 0) ? i - 2 : i);
      scanlines.mp = LINE2 (fieldp, i);
      scanlines.bbp = LINE2 (fieldp, (i + 2 < frame_height ? i + 2 : i));

      scanlines.t0 = LINE2 (field0, i - 1);
      scanlines.b0 = LINE2 (field0, i + 1);

      scanlines.tt1 = LINE2 (field1, (i - 2 >= 0) ? i - 2 : i);
      scanlines.m1 = LINE2 (field1, i);
      scanlines.bb1 = LINE2 (field1, (i + 2 < frame_height ? i + 2 : i));

      scanlines.t2 = LINE2 (field2, i - 1);
      scanlines.b2 = LINE2 (field2, i + 1);

      interpolate_scanline (self, LINE (dest, i), &scanlines);
    }
  }
}

static void
gst_deinterlace_simple_method_deinterlace_frame_planar (GstDeinterlaceMethod *
    method, const GstDeinterlaceField * history, guint history_count,
    GstVideoFrame * outframe, gint cur_field_idx)
{
  GstDeinterlaceSimpleMethod *self = GST_DEINTERLACE_SIMPLE_METHOD (method);
  GstDeinterlaceMethodClass *dm_class = GST_DEINTERLACE_METHOD_GET_CLASS (self);
  guint8 *out;
  const guint8 *field0, *field1, *field2, *fieldp;
  guint cur_field_flags = history[cur_field_idx].flags;
  gint i;
  GstDeinterlaceSimpleMethodFunction copy_scanline;
  GstDeinterlaceSimpleMethodFunction interpolate_scanline;

  g_assert (self->interpolate_scanline_planar[0] != NULL);
  g_assert (self->interpolate_scanline_planar[1] != NULL);
  g_assert (self->interpolate_scanline_planar[2] != NULL);
  g_assert (self->copy_scanline_planar[0] != NULL);
  g_assert (self->copy_scanline_planar[1] != NULL);
  g_assert (self->copy_scanline_planar[2] != NULL);

  for (i = 0; i < 3; i++) {
    copy_scanline = self->copy_scanline_planar[i];
    interpolate_scanline = self->interpolate_scanline_planar[i];

    out = GST_VIDEO_FRAME_PLANE_DATA (outframe, i);

    fieldp = NULL;
    if (cur_field_idx > 0) {
      fieldp = GST_VIDEO_FRAME_PLANE_DATA (history[cur_field_idx - 1].frame, i);
    }

    field0 = GST_VIDEO_FRAME_PLANE_DATA (history[cur_field_idx].frame, i);

    g_assert (dm_class->fields_required <= 4);

    field1 = NULL;
    if (cur_field_idx + 1 < history_count) {
      field1 = GST_VIDEO_FRAME_PLANE_DATA (history[cur_field_idx + 1].frame, i);
    }

    field2 = NULL;
    if (cur_field_idx + 2 < history_count) {
      field2 = GST_VIDEO_FRAME_PLANE_DATA (history[cur_field_idx + 2].frame, i);
    }

    gst_deinterlace_simple_method_deinterlace_frame_planar_plane (self, out,
        field0, field1, field2, fieldp, cur_field_flags, i, copy_scanline,
        interpolate_scanline);
  }
}

static void
gst_deinterlace_simple_method_deinterlace_frame_nv12 (GstDeinterlaceMethod *
    method, const GstDeinterlaceField * history, guint history_count,
    GstVideoFrame * outframe, gint cur_field_idx)
{
  GstDeinterlaceSimpleMethod *self = GST_DEINTERLACE_SIMPLE_METHOD (method);
  GstDeinterlaceMethodClass *dm_class = GST_DEINTERLACE_METHOD_GET_CLASS (self);
  guint8 *out;
  const guint8 *field0, *field1, *field2, *fieldp;
  guint cur_field_flags = history[cur_field_idx].flags;
  gint i;

  g_assert (self->interpolate_scanline_packed != NULL);
  g_assert (self->copy_scanline_packed != NULL);

  for (i = 0; i < 2; i++) {
    out = GST_VIDEO_FRAME_PLANE_DATA (outframe, i);

    fieldp = NULL;
    if (cur_field_idx > 0) {
      fieldp = GST_VIDEO_FRAME_PLANE_DATA (history[cur_field_idx - 1].frame, i);
    }

    field0 = GST_VIDEO_FRAME_PLANE_DATA (history[cur_field_idx].frame, i);

    g_assert (dm_class->fields_required <= 4);

    field1 = NULL;
    if (cur_field_idx + 1 < history_count) {
      field1 = GST_VIDEO_FRAME_PLANE_DATA (history[cur_field_idx + 1].frame, i);
    }

    field2 = NULL;
    if (cur_field_idx + 2 < history_count) {
      field2 = GST_VIDEO_FRAME_PLANE_DATA (history[cur_field_idx + 2].frame, i);
    }

    gst_deinterlace_simple_method_deinterlace_frame_planar_plane (self, out,
        field0, field1, field2, fieldp, cur_field_flags, i,
        self->copy_scanline_packed, self->interpolate_scanline_packed);
  }
}

static void
gst_deinterlace_simple_method_setup (GstDeinterlaceMethod * method,
    GstVideoInfo * vinfo)
{
  GstDeinterlaceSimpleMethod *self = GST_DEINTERLACE_SIMPLE_METHOD (method);
  GstDeinterlaceSimpleMethodClass *klass =
      GST_DEINTERLACE_SIMPLE_METHOD_GET_CLASS (self);

  GST_DEINTERLACE_METHOD_CLASS
      (gst_deinterlace_simple_method_parent_class)->setup (method, vinfo);

  self->interpolate_scanline_packed = NULL;
  self->copy_scanline_packed = NULL;

  self->interpolate_scanline_planar[0] = NULL;
  self->interpolate_scanline_planar[1] = NULL;
  self->interpolate_scanline_planar[2] = NULL;
  self->copy_scanline_planar[0] = NULL;
  self->copy_scanline_planar[1] = NULL;
  self->copy_scanline_planar[2] = NULL;

  if (GST_VIDEO_INFO_FORMAT (vinfo) == GST_VIDEO_FORMAT_UNKNOWN)
    return;

  switch (GST_VIDEO_INFO_FORMAT (vinfo)) {
    case GST_VIDEO_FORMAT_YUY2:
      self->interpolate_scanline_packed = klass->interpolate_scanline_yuy2;
      self->copy_scanline_packed = klass->copy_scanline_yuy2;
      break;
    case GST_VIDEO_FORMAT_YVYU:
      self->interpolate_scanline_packed = klass->interpolate_scanline_yvyu;
      self->copy_scanline_packed = klass->copy_scanline_yvyu;
      break;
    case GST_VIDEO_FORMAT_UYVY:
      self->interpolate_scanline_packed = klass->interpolate_scanline_uyvy;
      self->copy_scanline_packed = klass->copy_scanline_uyvy;
      break;
    case GST_VIDEO_FORMAT_AYUV:
      self->interpolate_scanline_packed = klass->interpolate_scanline_ayuv;
      self->copy_scanline_packed = klass->copy_scanline_ayuv;
      break;
    case GST_VIDEO_FORMAT_ARGB:
    case GST_VIDEO_FORMAT_xRGB:
      self->interpolate_scanline_packed = klass->interpolate_scanline_argb;
      self->copy_scanline_packed = klass->copy_scanline_argb;
      break;
    case GST_VIDEO_FORMAT_ABGR:
    case GST_VIDEO_FORMAT_xBGR:
      self->interpolate_scanline_packed = klass->interpolate_scanline_abgr;
      self->copy_scanline_packed = klass->copy_scanline_abgr;
      break;
    case GST_VIDEO_FORMAT_RGBA:
    case GST_VIDEO_FORMAT_RGBx:
      self->interpolate_scanline_packed = klass->interpolate_scanline_rgba;
      self->copy_scanline_packed = klass->copy_scanline_rgba;
      break;
    case GST_VIDEO_FORMAT_BGRA:
    case GST_VIDEO_FORMAT_BGRx:
      self->interpolate_scanline_packed = klass->interpolate_scanline_bgra;
      self->copy_scanline_packed = klass->copy_scanline_bgra;
      break;
    case GST_VIDEO_FORMAT_RGB:
      self->interpolate_scanline_packed = klass->interpolate_scanline_rgb;
      self->copy_scanline_packed = klass->copy_scanline_rgb;
      break;
    case GST_VIDEO_FORMAT_BGR:
      self->interpolate_scanline_packed = klass->interpolate_scanline_bgr;
      self->copy_scanline_packed = klass->copy_scanline_bgr;
      break;
    case GST_VIDEO_FORMAT_NV12:
      self->interpolate_scanline_packed = klass->interpolate_scanline_nv12;
      self->copy_scanline_packed = klass->copy_scanline_nv12;
      break;
    case GST_VIDEO_FORMAT_NV21:
      self->interpolate_scanline_packed = klass->interpolate_scanline_nv21;
      self->copy_scanline_packed = klass->copy_scanline_nv21;
      break;
    case GST_VIDEO_FORMAT_I420:
    case GST_VIDEO_FORMAT_YV12:
    case GST_VIDEO_FORMAT_Y444:
    case GST_VIDEO_FORMAT_Y42B:
    case GST_VIDEO_FORMAT_Y41B:
      self->interpolate_scanline_planar[0] =
          klass->interpolate_scanline_planar_y;
      self->copy_scanline_planar[0] = klass->copy_scanline_planar_y;
      self->interpolate_scanline_planar[1] =
          klass->interpolate_scanline_planar_u;
      self->copy_scanline_planar[1] = klass->copy_scanline_planar_u;
      self->interpolate_scanline_planar[2] =
          klass->interpolate_scanline_planar_v;
      self->copy_scanline_planar[2] = klass->copy_scanline_planar_v;
      break;
    default:
      break;
  }
}

static void
gst_deinterlace_simple_method_class_init (GstDeinterlaceSimpleMethodClass
    * klass)
{
  GstDeinterlaceMethodClass *dm_class = (GstDeinterlaceMethodClass *) klass;

  dm_class->deinterlace_frame_ayuv =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_yuy2 =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_yvyu =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_uyvy =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_argb =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_abgr =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_rgba =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_bgra =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_rgb =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_bgr =
      gst_deinterlace_simple_method_deinterlace_frame_packed;
  dm_class->deinterlace_frame_i420 =
      gst_deinterlace_simple_method_deinterlace_frame_planar;
  dm_class->deinterlace_frame_yv12 =
      gst_deinterlace_simple_method_deinterlace_frame_planar;
  dm_class->deinterlace_frame_y444 =
      gst_deinterlace_simple_method_deinterlace_frame_planar;
  dm_class->deinterlace_frame_y42b =
      gst_deinterlace_simple_method_deinterlace_frame_planar;
  dm_class->deinterlace_frame_y41b =
      gst_deinterlace_simple_method_deinterlace_frame_planar;
  dm_class->deinterlace_frame_nv12 =
      gst_deinterlace_simple_method_deinterlace_frame_nv12;
  dm_class->deinterlace_frame_nv21 =
      gst_deinterlace_simple_method_deinterlace_frame_nv12;
  dm_class->fields_required = 2;
  dm_class->setup = gst_deinterlace_simple_method_setup;
  dm_class->supported = gst_deinterlace_simple_method_supported;

  klass->interpolate_scanline_yuy2 =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_yuy2 =
      gst_deinterlace_simple_method_copy_scanline_packed;
  klass->interpolate_scanline_yvyu =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_yvyu =
      gst_deinterlace_simple_method_copy_scanline_packed;
  klass->interpolate_scanline_ayuv =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_ayuv =
      gst_deinterlace_simple_method_copy_scanline_packed;
  klass->interpolate_scanline_uyvy =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_uyvy =
      gst_deinterlace_simple_method_copy_scanline_packed;
  klass->interpolate_scanline_nv12 =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_nv12 =
      gst_deinterlace_simple_method_copy_scanline_packed;

  klass->interpolate_scanline_argb =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_argb =
      gst_deinterlace_simple_method_copy_scanline_packed;
  klass->interpolate_scanline_abgr =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_abgr =
      gst_deinterlace_simple_method_copy_scanline_packed;

  klass->interpolate_scanline_rgba =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_rgba =
      gst_deinterlace_simple_method_copy_scanline_packed;
  klass->interpolate_scanline_bgra =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_bgra =
      gst_deinterlace_simple_method_copy_scanline_packed;
  klass->interpolate_scanline_rgb =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_rgb = gst_deinterlace_simple_method_copy_scanline_packed;
  klass->interpolate_scanline_bgr =
      gst_deinterlace_simple_method_interpolate_scanline_packed;
  klass->copy_scanline_bgr = gst_deinterlace_simple_method_copy_scanline_packed;

  klass->interpolate_scanline_planar_y =
      gst_deinterlace_simple_method_interpolate_scanline_planar_y;
  klass->copy_scanline_planar_y =
      gst_deinterlace_simple_method_copy_scanline_planar_y;
  klass->interpolate_scanline_planar_u =
      gst_deinterlace_simple_method_interpolate_scanline_planar_u;
  klass->copy_scanline_planar_u =
      gst_deinterlace_simple_method_copy_scanline_planar_u;
  klass->interpolate_scanline_planar_v =
      gst_deinterlace_simple_method_interpolate_scanline_planar_v;
  klass->copy_scanline_planar_v =
      gst_deinterlace_simple_method_copy_scanline_planar_v;
}

static void
gst_deinterlace_simple_method_init (GstDeinterlaceSimpleMethod * self)
{
}
