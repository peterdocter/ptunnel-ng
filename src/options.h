#ifndef OPTIONS_H
#define OPTIONS_H 1

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#ifndef WIN32
#include <pwd.h>
#include <grp.h>
#endif
#ifdef HAVE_SELINUX
#include <selinux/selinux.h>
#endif

#include "md5.h"
#include "pconfig.h"

struct options {
	/** user defined magic value (prevent Cisco WSA/IronPort fingerprint scan) */
	uint32_t magic;
	/** proxy or forwarder? */
	int mode;
	/** Proxy's internet address */
	char *given_proxy_hostname;
	uint32_t given_proxy_ip;
	/** Port the client listens on */
	uint32_t tcp_listen_port;
	/** restrict Forward/Proxy destination internet address */
	int restrict_dst_ip;
	char *given_dst_hostname;
	uint32_t given_dst_ip;
	/** restrict Forward/Proxy destination port */
	int restrict_dst_port;
	uint32_t given_dst_port;
	/** Default maximum number of tunnels to support at once */
	uint32_t max_tunnels;
	/** Default log level */
	int log_level;
#ifdef HAVE_PCAP
	/** Non zero value if user wants packet capturing */
	int pcap;
	/** Device to capture packets from */
	char *pcap_device;
#endif
	/** Usually stdout, but can be altered by the user */
	char *log_path;
	FILE *log_file;
	/** Print more detailed traffic statistics if non zero value */
	int print_stats;
	/** Password (must be the same on proxy and client for authentica  tion to succeed) */
	char *password;
	/** MD5 digest of challenge+password */
	md5_byte_t password_digest[kMD5_digest_size];
	/** use UDP instead of ICMP */
	int udp;
	/** unpriviledged mode */
	int unprivileged;

#ifndef WIN32
	/** run as daemon if non zero value */
	int daemonize;
	/** PIDFILE if running as daemon */
	char *pid_path;
	FILE *pid_file;
	/** log to syslog if non zero value */
	int use_syslog;
	/** UID of the running process */
	uid_t uid;
	/** GID of the running process */
	gid_t gid;
	/** CHROOT dir */
	int chroot;
	char *root_dir;
#endif

#ifdef HAVE_SELINUX
	/** Non zero value if uer wants SeLinux */
	int selinux;
	/** SeLinux context name */
	char *selinux_context;
#endif
};

extern struct options opts;

void print_usage(const char *arg0);

int parse_options(int argc, char **argv);

#endif
