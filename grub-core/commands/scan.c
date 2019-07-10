/* scan.c - command to probe for additional devices */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2003,2005,2007,2008,2009  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/err.h>
#include <grub/dl.h>
#include <grub/disk.h>
#include <grub/device.h>
#include <grub/term.h>
#include <grub/partition.h>
#include <grub/file.h>
#include <grub/normal.h>
#include <grub/extcmd.h>
#include <grub/datetime.h>
#include <grub/i18n.h>
#include <grub/net.h>

GRUB_MOD_LICENSE ("GPLv3+");

static const struct grub_arg_option options[] =
  {
    {0, 0, 0, 0, 0, 0}
  };

typedef struct grub_scan_devices {
  const char * type;
} grub_scan_devices_t;

/* Helper for grub_ls_list_devices.  */
static int
grub_scan_device_type (const grub_disk_dev_t dev, void *data)
{
  grub_scan_devices_t * p = (grub_scan_devices_t *) data;

  if (dev->disk_scan) {
    grub_xputs ("Scanning ");
    grub_xputs (dev->name);
    if (p->type) {
      grub_xputs (" for ");
      grub_xputs (p->type);
    }
    dev->disk_scan (p->type);
    grub_xputs ("\n");
  }

  return 0;
}

static grub_err_t
grub_scan_devices (const char * type)
{
  grub_scan_devices_t data = { .type = type };
  grub_disk_drv_iterate (grub_scan_device_type, (void *)&data);
  grub_xputs ("\n");

  grub_refresh ();

  return 0;
}

static grub_err_t
grub_cmd_scan (grub_extcmd_context_t ctxt, int argc, char **args)
{
  int i = 0;

  (void) ctxt; /* FIXME what is this for? */
  do {
    grub_scan_devices (args[i]);
    i++;
  } while (i < argc);

  return 0;
}

static grub_extcmd_t cmd;

GRUB_MOD_INIT(scan)
{
  cmd = grub_register_extcmd ("scan", grub_cmd_scan, 0,
			      N_("[TYPE]"),
			      N_("Scan for more disk devices."), options);
}

GRUB_MOD_FINI(scan)
{
  grub_unregister_extcmd (cmd);
}
