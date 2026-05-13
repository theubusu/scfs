/* With non GPL files, use following license */
/*
 * Copyright 2005-2007 Sony Corporation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/* Otherwise with GPL files, use following license */
/*
 * Copyright 2005-2007 Sony Corporation.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License.
 *
 * THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 * WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 * USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * scfs_plug_in_lib.h
 */

#ifndef _SCFS_PLUG_IN_LIB_H_
#define _SCFS_PLUG_IN_LIB_H_

/*
 * This macro is used for debugging
 */
#undef PDEBUG
#ifdef SCFS_DEBUG
#	ifdef __KERNEL__	/* If process is in kernel space */
#		define PDEBUG(fmt, args...) printk(KERN_ERR fmt, ## args)
#	else			/* If process is in user space */
#		include <stdio.h>
#		define PDEBUG(fmt, args...) fprintf(stdout, fmt, ## args)
#	endif /* !__KERNEL__ */
#else	/* If It is not debugging mode, this macro do nothing */
#	define PDEBUG(fmt, args...)
#endif	/* !PDEBUG */

/*
 * This macro is used for error message.
 */
#ifdef __KERNEL__	/* If process is in kernel space */
#	define SCFS_PLUGIN_ERR(fmt, args...)			\
		printk(KERN_ERR "#error %s(): "fmt,		\
			__FUNCTION__, ##args)
#else			/* If process is in user space */
#	include <stdio.h>
#	define SCFS_PLUGIN_ERR(fmt, args...)			\
		fprintf(stderr, "#error %s(): "fmt,		\
			__FUNCTION__, ##args)
#endif /* !__KERNEL__ */

/* Bit width of a member of the plug-in id */
#define SCFS_PLUGINID_FUNC_BITS		2
#define SCFS_PLUGINID_MAJOR_BITS	14
#define SCFS_PLUGINID_IF_BITS		8
#define SCFS_PLUGINID_MINOR_BITS	8
/* Bit shift for a member of the plug-in id */
#define SCFS_PLUGINID_FUNC_SHIFT	\
		(SCFS_PLUGINID_MAJOR_BITS + SCFS_PLUGINID_IF_BITS + \
		 SCFS_PLUGINID_MINOR_BITS)
#define SCFS_PLUGINID_MAJOR_SHIFT	\
		(SCFS_PLUGINID_IF_BITS + SCFS_PLUGINID_MINOR_BITS)
#define SCFS_PLUGINID_IF_SHIFT	(SCFS_PLUGINID_MINOR_BITS)
#define SCFS_PLUGINID_MINOR_SHIFT	0
/* Bit mask for a member of the plug-in id */
#define SCFS_PLUGINID_FUNC_MASK		\
		(((1 << SCFS_PLUGINID_FUNC_BITS) - 1) << \
		 SCFS_PLUGINID_FUNC_SHIFT)
#define SCFS_PLUGINID_MAJOR_MASK	\
		(((1 << SCFS_PLUGINID_MAJOR_BITS) - 1) << \
		 SCFS_PLUGINID_MAJOR_SHIFT)
#define SCFS_PLUGINID_IF_MASK		\
		(((1 << SCFS_PLUGINID_IF_BITS) - 1) << \
		 SCFS_PLUGINID_IF_SHIFT)
#define SCFS_PLUGINID_MINOR_MASK	\
		(((1 << SCFS_PLUGINID_MINOR_BITS) - 1) << \
		 SCFS_PLUGINID_MINOR_SHIFT)

/*
 * Definitions for the setting encparam & compparam
 */
#define SCFS_ENDIAN_CONVERT_ON		1
#define IS_SCFS_ENDIAN_CONVERT(x)	((x) & SCFS_ENDIAN_CONVERT_ON)


/* useful macros for plug-in */
#define SCFS_CONVERT_ENDIAN_U32(x) \
	((((x) << 24) & 0xff000000) | \
	 (((x) << 8)  & 0x00ff0000) | \
	 (((x) >> 8)  & 0x0000ff00) | \
	 (((x) >> 24) & 0x000000ff))

/* x is data, y is flag */
#define SCFS_SET_DATA_U32(x, y)	\
	((IS_SCFS_ENDIAN_CONVERT(y) == 0) ? (x) : \
	 SCFS_CONVERT_ENDIAN_U32((x)))

#define SCFS_SET_PLUG_IN_ID(func, major, interface, minor) \
((((func)      << SCFS_PLUGINID_FUNC_SHIFT)  & SCFS_PLUGINID_FUNC_MASK)   | \
 (((major)     << SCFS_PLUGINID_MAJOR_SHIFT) & SCFS_PLUGINID_MAJOR_MASK)  | \
 (((interface) << SCFS_PLUGINID_IF_SHIFT)    & SCFS_PLUGINID_IF_MASK)     | \
 (((minor)     << SCFS_PLUGINID_MINOR_SHIFT) & SCFS_PLUGINID_MINOR_MASK))

#define SCFS_GET_PLUGINID_FUNC(x)	((x) >> SCFS_PLUGINID_FUNC_SHIFT)
#define SCFS_GET_PLUGINID_MAJOR(x)	(((x) & SCFS_PLUGINID_MAJOR_MASK) >> \
					 SCFS_PLUGINID_MAJOR_SHIFT)
#define SCFS_GET_PLUGINID_IF(x)		(((x) & SCFS_PLUGINID_IF_MASK) >> \
					 SCFS_PLUGINID_IF_SHIFT)
#define SCFS_GET_PLUGINID_MINOR(x)	(((x) & SCFS_PLUGINID_MINOR_MASK) >> \
					 SCFS_PLUGINID_MINOR_SHIFT)


#endif  /* !_SCFS_PLUG_IN_LIB_H_ */
