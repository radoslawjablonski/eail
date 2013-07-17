/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef EAIL_PRIV_H
#define EAIL_PRIV_H

#define EAIL_LOG_COLOR EINA_COLOR_GREEN

#ifdef CRITICAL
#undef CRITICAL
#endif
#define CRITICAL(...) EINA_LOG_DOM_CRIT(_eail_log_dom, __VA_ARGS__)
#ifdef ERR
#undef ERR
#endif
#define ERR(...)      EINA_LOG_DOM_ERR(_eail_log_dom, __VA_ARGS__)
#ifdef WRN
#undef WRN
#endif
#define WRN(...)      EINA_LOG_DOM_WARN(_eail_log_dom, __VA_ARGS__)
#ifdef INF
#undef INF
#endif
#define INF(...)      EINA_LOG_DOM_INFO(_eail_log_dom, __VA_ARGS__)
#ifdef DBG
#undef DBG
#endif
#define DBG(...)      EINA_LOG_DOM_DBG(_eail_log_dom, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

extern int _eail_log_dom;

#ifdef __cplusplus
}
#endif

#endif
