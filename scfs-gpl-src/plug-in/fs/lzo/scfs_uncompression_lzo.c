/*
 *  Copyright 2007,2008 Sony Corporation.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  version 2 of the  License.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * scfs_uncompression_lzo.c
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/lzo.h>
#include <scfs_plug_in_lib.h>
#include <scfs_plug_in_info_ucomp.h>
#include <scfs_plug_in_lzo.h>

static int
scfs_uncompress_chk_compparam(const void *pcompparam)
{

	if (pcompparam != NULL) {
		SCFS_PLUGIN_ERR("Invalid compparam.\n");
		return -EINVAL;
	}
	return 0;
}

static int
scfs_uncompress_data(void *dest, size_t *destlen, const void *src,
		     size_t srclen, void *pcompparam)
{
	int ret;

	ret = lzo1x_decompress_safe(src, srclen, dest, destlen);
	if (ret < 0) {
		SCFS_PLUGIN_ERR("failed lzo decompression: "
				"lzo errno = %d\n", ret);
		return -EINVAL;
	}

	return 0;
}

static int
scfs_uncompress_chk_id(u_int32_t id)
{

	if (SCFS_GET_PLUGINID_FUNC(id)  != SCFS_PLUGINID_FUNC_COMP   ||
	    SCFS_GET_PLUGINID_MAJOR(id) != SCFS_PLUGINID_MAJOR_LZO   ||
	    SCFS_GET_PLUGINID_IF(id)    != SCFS_PLUGINID_IF_LZO_COMP ||
	    SCFS_GET_PLUGINID_MINOR(id)  > SCFS_PLUGINID_MINOR_LZO_COMP) {
		return -EINVAL;
	}

	return 0;
}

static int
scfs_uncompress_set_opt(int argc, char **argv,  void *pcompparam)
{

	if (argc > 0) {
		SCFS_PLUGIN_ERR("invalid argument.\n");
		return -EINVAL;
	}
	return 0;
}

static int
scfs_uncompress_max_data_len(size_t *len, size_t size)
{

	*len = SCFS_LZO_MAX_COMP_SIZE(size);
	return 0;
}

static scfs_ucomp_plugin_info_t plugin_info = {
	.header.id = SCFS_SET_PLUG_IN_ID(SCFS_PLUGINID_FUNC_UNCOMP,
					 SCFS_PLUGINID_MAJOR_LZO,
					 SCFS_PLUGINID_IF_LZO_UNCOMP,
					 SCFS_PLUGINID_MINOR_LZO_UNCOMP),
	.header.module = THIS_MODULE,
	.ops.chk_compparam = scfs_uncompress_chk_compparam,
	.ops.uncompress_data = scfs_uncompress_data,
	.ops.chk_id = scfs_uncompress_chk_id,
	.ops.set_opt = scfs_uncompress_set_opt,
	.ops.max_data_len = scfs_uncompress_max_data_len,
};

static int __init
scfs_uncompression_lzo_init(void)
{
	int ret;

	PDEBUG("IN : %s\n", __FUNCTION__);

	ret = scfs_register_ucomp_plugin(&plugin_info);
	if (ret < 0) {
		return ret;
	}

	PDEBUG("OUT : %s\n", __FUNCTION__);
	return 0;
}

static void __exit
scfs_uncompression_lzo_exit(void)
{

	PDEBUG("IN : %s\n", __FUNCTION__);

	scfs_unregister_ucomp_plugin(&plugin_info);

	PDEBUG("OUT : %s\n", __FUNCTION__);
	return;
}

module_init(scfs_uncompression_lzo_init);
module_exit(scfs_uncompression_lzo_exit);

MODULE_DESCRIPTION("Sony SCFS Uncompression plug-in : lzo");
MODULE_AUTHOR("Sony Corporation");
MODULE_LICENSE("GPL");
