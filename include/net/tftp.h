#pragma once

#include <block.h>

// fixme
#ifndef MAX_FILE_NAME_LEN
#define MAX_FILE_NAME_LEN   64
#endif

#define TFTP_RRQ   CPU_TO_BE16(1)
#define TFTP_WRQ   CPU_TO_BE16(2)
#define TFTP_DAT   CPU_TO_BE16(3)
#define TFTP_ACK   CPU_TO_BE16(4)
#define TFTP_ERR   CPU_TO_BE16(5)

#define TFTP_HDR_LEN   4
#define TFTP_PKT_LEN   512
#define TFTP_BUF_LEN  (TFTP_PKT_LEN + TFTP_HDR_LEN)

#define TFTP_MODE_OCTET  "octet"

#undef  TFTP_DEBUG  // fixme: depend on configuration
#define TFTP_VERBOSE

#define MAX_MODE_LEN 12
#define MAX_PATH_LEN 12

struct tftp_opt {
	bool  verbose;
	__u32 server_ip;
	char  file_name[MAX_FILE_NAME_LEN];
	char  path[MAX_PATH_LEN];
	char  mode[MAX_MODE_LEN];
	void  *load_addr;
	size_t xmit_size;
	const char *type; // only for image
	struct bdev_file *file;
};

int tftp_download(struct tftp_opt *opt);
int tftp_upload(struct tftp_opt *opt);
PART_TYPE str2part_type(const char *str);
