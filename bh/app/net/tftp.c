#include <g-bios.h>
#include <net/tftp.h>
#include <getopt.h>
#include <flash/part.h>
#include <sysconf.h>

static char app_option[][CMD_OPTION_LEN] = {"a", "m", "s", "h", "0"};

static void tftp_usage(void)
{
		printf("Usage: tftp [option <value>] [-f] <filename>\n"
		"\noptions:\n"
		"  -a   \tmemory address [needed in few cases]\n"
		"  -m   \tonly download to memory, without writing to storage\n"
//		"  -f   \tfile name for download\n"
		"  -s   \ttftp server ip\n"
		"  -h   \tthis help\n");
}


int main(int argc, char *argv[])
{
	int ret, ch;
	struct tftp_opt dlopt;
	BOOL mem_only = FALSE;
	char *opt_arg;
	int opt_idx;
	struct partition *part = NULL;

	memset(&dlopt, 0x0, sizeof(dlopt));
	net_get_server_ip(&dlopt.server_ip);

	while ((ch = getopt(argc, argv, "a:m:s:f:", &opt_arg)) != -1)
	{
		switch(ch)
		{
		case 'a':
			ret = string2value(opt_arg, (u32 *)&dlopt.load_addr);

			if (ret < 0)
			{
				tftp_usage();
				return ret;
			}

			break;

		case 'm':
			mem_only = TRUE;
			break;

		case 's':
			ret = str_to_ip((u8 *)&dlopt.server_ip, opt_arg);
			if (ret < 0)
			{
				printf("Invalid IP: %s!\n", opt_arg);
				return ret;
			}
			break;

		case 'f':
			strncpy(dlopt.file_name, opt_arg, MAX_FILE_NAME_LEN);
			dlopt.file_name[MAX_FILE_NAME_LEN - 1] = '\0';
			break;

		case 'h':
		default:
			tftp_usage();
			return -EINVAL;
		}
	}

	if (FALSE == mem_only)
	{
		part = part_open(PART_CURR, OP_RDWR);

		if (NULL == part)
		{
			printf("Invalid partition!\n");
			return -EINVAL;
		}

		dlopt.part = part;
	}

	opt_idx = getopt_idx();

	if (opt_idx < argc)
	{
		if (opt_idx + 1 == argc && !dlopt.file_name[0])
		{
			strncpy(dlopt.file_name, argv[opt_idx], MAX_FILE_NAME_LEN);
			dlopt.file_name[MAX_FILE_NAME_LEN - 1] = '\0';
		}
		else
		{
			tftp_usage();
			return -EINVAL;
		}
	}

	if (!dlopt.file_name[0])
	{
		u32 size;

		if (NULL == part)
		{
			part = part_open(PART_CURR, OP_RDONLY);
			if (NULL == part)
			{
				tftp_usage();
				return -EINVAL;
			}
		}

		part_get_image(part, dlopt.file_name, &size);

		// fixme
		if (!dlopt.file_name[0] || size >= part_get_size(part))
		{
			switch (part_get_type(part))
			{
			case PT_BL_GTH:
				strcpy(dlopt.file_name, "g-bios-th.bin");
				break;

			case PT_BL_GBH:
				strcpy(dlopt.file_name, "g-bios-bh.bin");
				break;

			case PT_BL_UBOOT:
				strcpy(dlopt.file_name, "u-boo.bin");
				break;

			case PT_OS_LINUX:
				strcpy(dlopt.file_name, "zImage");
				break;

			case PT_FS_JFFS2:
				// fixme for small page
				strcpy(dlopt.file_name, "rootfs_l.jffs2");
				break;
			// TODO: support other images

			default:
				printf("no file name specified!\n");
				return -EINVAL;
			}
		}
	}

	ret = net_tftp_load(&dlopt);

	// if (part)
		part_close(part);

	if (FALSE == mem_only)
	{
		if (ret > 0)
		{
			part_set_image(dlopt.part, dlopt.file_name, ret);
			sysconf_save();
		}
	}

	return ret;
}
