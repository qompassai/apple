// /qompassai/apple/osxcross/xar/xar.h
// --------------------------------------
// Copyright (C) 2025 Qompass AI, All rights reserved
 
#ifndef _XAR_H_
#define _XAR_H_

#define XAR_VERSION "1.6.1"
#define XAR_VERSION_NUM 0x01060180

#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(4)

struct xar_header {
	uint32_t magic;
	uint16_t size;
	uint16_t version;
	uint64_t toc_length_compressed;
	uint64_t toc_length_uncompressed;
	uint32_t cksum_alg;
};

struct xar_header_ex {
	uint32_t magic;
	uint16_t size;
	uint16_t version;
	uint64_t toc_length_compressed;
	uint64_t toc_length_uncompressed;
	uint32_t cksum_alg;
	char     toc_cksum_name[36];	/* Nul terminated and padded toc checksum style attribute value. */
					/* Must not be "none" or empty string "" if cksum_alg == XAR_CKSUM_OTHER */
};

#pragma pack()

typedef struct xar_header xar_header_t;
typedef struct xar_header_ex xar_header_ex_t;

#define XAR_HEADER_MAGIC 0x78617221
#define XAR_EA_FORK "ea"

#define XAR_CKSUM_NONE   0
#define XAR_CKSUM_SHA1   1
#define XAR_CKSUM_MD5    2
#define XAR_CKSUM_OTHER  3

typedef void *xar_errctx_t;
typedef const struct __xar_file_t *xar_file_t;
typedef const struct __xar_iter_t *xar_iter_t;
typedef const struct __xar_t *xar_t;
typedef const struct __xar_subdoc_t *xar_subdoc_t;
typedef const struct __xar_signature_t *xar_signature_t;

typedef struct {
        char *next_out;
        unsigned int avail_out;

        unsigned long long total_in;
        unsigned long long total_out;

        void *state;
} xar_stream;

typedef int32_t (*err_handler)(int32_t severit, int32_t instance, xar_errctx_t ctx, void *usrctx);
/* the signed_data must be allocated durring the callback and will be released by the xar lib after the callback */
typedef int32_t (*xar_signer_callback)(xar_signature_t sig, void *context, uint8_t *data, uint32_t length, uint8_t **signed_data, uint32_t *signed_len);

#define READ 0
#define WRITE 1

#define XAR_STREAM_OK   0
#define XAR_STREAM_END  1
#define XAR_STREAM_ERR -1

#define XAR_OPT_OWNERSHIP    "ownership"  /* setting owner/group behavior */
#define XAR_OPT_VAL_SYMBOLIC "symbolic"   /* set owner/group based on names */
#define XAR_OPT_VAL_NUMERIC  "numeric"    /* set owner/group based on uid/gid */

#define XAR_OPT_TOCCKSUM	"toc-cksum"	/* set the toc checksum algorithm */
#define XAR_OPT_FILECKSUM	"file-chksum"	/* set the file checksum algorithm */
#define XAR_OPT_VAL_NONE   "none"
#define XAR_OPT_VAL_MD5    "md5"
#define XAR_OPT_VAL_SHA1   "sha1"
#define XAR_OPT_VAL_SHA224 "sha224"
#define XAR_OPT_VAL_SHA256 "sha256"
#define XAR_OPT_VAL_SHA384 "sha384"
#define XAR_OPT_VAL_SHA512 "sha512"
/* Actually any valid hash function name that returns non-NULL from EVP_get_digestbyname can be used as a value */

#define XAR_OPT_COMPRESSION    "compression" /* set the file compression type */
#define XAR_OPT_COMPRESSIONARG "compression-arg" /* set the compression opts */
#define XAR_OPT_VAL_GZIP       "gzip"
#define XAR_OPT_VAL_BZIP       "bzip2"
#define XAR_OPT_VAL_LZMA       "lzma"
#define XAR_OPT_VAL_XZ         "xz"

#define XAR_OPT_RSIZE          "rsize"       /* Read io buffer size */

#define XAR_OPT_COALESCE       "coalesce"    /* Coalesce identical heap blocks */
#define XAR_OPT_LINKSAME       "linksame"    /* Hardlink identical files */

#define XAR_OPT_PROPINCLUDE    "prop-include" /* File property to include */
#define XAR_OPT_PROPEXCLUDE    "prop-exclude" /* File property to exclude */

#define XAR_OPT_SAVESUID       "savesuid"     /* Preserve setuid/setgid bits */
#define XAR_OPT_VAL_TRUE       "true"
#define XAR_OPT_VAL_FALSE      "false"

#define XAR_OPT_RECOMPRESS     "recompress"   /* Allow recompression (true/false) */

#define XAR_OPT_EXTRACTSTDOUT  "extract-stdout" /* Extract data to stdout instead of file (true/false) */

#define XAR_OPT_STRIPCOMPONENTS "strip-components" /* Number of components to strip on extraction (default 0) */

/* Use xar_open("",WRITE) to get an xar_t to use xar_opt_get on to fetch this then xar_close it if an xar_t is not otherwise available */
/* Older versions will return NULL from xar_open("",WRITE), current versions will not but only xar_opt_get and xar_close should be called on the result */
#define XAR_OPT_XARLIBVERSION   "xar-library-version" /* Library's XAR_VERSION_NUM (as 0xVALUE string [%i scanf format, strtol base 0], get only) */

/* Note that this option is forced to true whenever XAR_CKSUM_OTHER is in effect */
#define XAR_OPT_RFC6713FORMAT  "rfc6713-format" /* Generate application/zlib instead of application/x-gzip encoding styles (true/false) */

/* xar signing algorithms */
#define XAR_SIG_SHA1RSA		1


/* xar error handler macros */
#define XAR_SEVERITY_DEBUG    1
#define XAR_SEVERITY_INFO     2
#define XAR_SEVERITY_NORMAL   3
#define XAR_SEVERITY_WARNING  4
#define XAR_SEVERITY_NONFATAL 5
#define XAR_SEVERITY_FATAL    6

#define XAR_ERR_ARCHIVE_CREATION   1
#define XAR_ERR_ARCHIVE_EXTRACTION 2

xar_t xar_open(const char *file, int32_t flags);
int xar_close(xar_t x);
xar_file_t xar_add(xar_t x, const char *path);

xar_file_t xar_add_frombuffer(xar_t x, xar_file_t parent, const char *name, char *buffer, size_t length);
xar_file_t xar_add_folder(xar_t x, xar_file_t f, const char *name, struct stat *info);
xar_file_t xar_add_frompath(xar_t x, xar_file_t parent, const char *name, const char *realpath);

xar_file_t xar_add_from_archive(xar_t x, xar_file_t parent, const char *name, xar_t sourcearchive, xar_file_t sourcefile);

int32_t xar_extract(xar_t x, xar_file_t f);
int32_t xar_extract_tofile(xar_t x, xar_file_t f, const char *path);
int32_t xar_extract_tobuffer(xar_t x, xar_file_t f, char **buffer);
int32_t xar_extract_tobuffersz(xar_t x, xar_file_t f, char **buffer, size_t *size);
int32_t xar_extract_tostream_init(xar_t x, xar_file_t f, xar_stream *stream);
int32_t xar_extract_tostream(xar_stream *stream);
int32_t xar_extract_tostream_end(xar_stream *stream);

int32_t xar_verify(xar_t x, xar_file_t f);


const char *xar_opt_get(xar_t x, const char *option);
int32_t xar_opt_set(xar_t x, const char *option, const char *value);
int32_t xar_opt_unset(xar_t x, const char *option);

int32_t xar_prop_set(xar_file_t f, const char *key, const char *value);
int32_t xar_prop_create(xar_file_t f, const char *key, const char *value);
int32_t xar_prop_get(xar_file_t f, const char *key, const char **value);

xar_iter_t xar_iter_new(void);
void xar_iter_free(xar_iter_t i);

const char *xar_prop_first(xar_file_t f, xar_iter_t i);
const char *xar_prop_next(xar_iter_t i);

void xar_prop_unset(xar_file_t f, const char *key);
xar_file_t xar_file_first(xar_t x, xar_iter_t i);
xar_file_t xar_file_next(xar_iter_t i);

const char *xar_attr_get(xar_file_t f, const char *prop, const char *key);
int32_t xar_attr_set(xar_file_t f, const char *prop, const char *key, const char *value);
const char *xar_attr_first(xar_file_t f, const char *prop, xar_iter_t i);
const char *xar_attr_next(xar_iter_t i);

xar_subdoc_t xar_subdoc_new(xar_t x, const char *name);
int32_t xar_subdoc_prop_set(xar_subdoc_t s, const char *key, const char *value);
int32_t xar_subdoc_prop_get(xar_subdoc_t s, const char *key, const char **value);
int32_t xar_subdoc_attr_set(xar_subdoc_t s, const char *prop, const char *key, const char *value);
const char *xar_subdoc_attr_get(xar_subdoc_t s, const char *prop, const char *key);
xar_subdoc_t xar_subdoc_first(xar_t x);
xar_subdoc_t xar_subdoc_next(xar_subdoc_t s);
const char *xar_subdoc_name(xar_subdoc_t s);
int32_t xar_subdoc_copyout(xar_subdoc_t s, unsigned char **, unsigned int *);
int32_t xar_subdoc_copyin(xar_subdoc_t s, const unsigned char *, unsigned int);
void xar_subdoc_remove(xar_subdoc_t s);

/* signature api for adding various signature types */
xar_signature_t xar_signature_new(xar_t x,const char *type, int32_t length, xar_signer_callback callback, void *callback_context);

const char *xar_signature_type(xar_signature_t s);

xar_signature_t xar_signature_first(xar_t x);
xar_signature_t xar_signature_next(xar_signature_t s);

int32_t xar_signature_add_x509certificate(xar_signature_t sig, const uint8_t *cert_data, uint32_t cert_len );

int32_t xar_signature_get_x509certificate_count(xar_signature_t sig);
int32_t xar_signature_get_x509certificate_data(xar_signature_t sig, int32_t index, const uint8_t **cert_data, uint32_t *cert_len);

int32_t xar_signature_copy_signed_data(xar_signature_t sig, uint8_t **data, uint32_t *length, uint8_t **signed_data, uint32_t *signed_length, uint64_t *signed_offset);

char *xar_get_size(xar_t x, xar_file_t f);
char *xar_get_type(xar_t x, xar_file_t f);
char *xar_get_mode(xar_t x, xar_file_t f);
char *xar_get_owner(xar_t x, xar_file_t f);
char *xar_get_group(xar_t x, xar_file_t f);
char *xar_get_mtime(xar_t x, xar_file_t f);

void xar_register_errhandler(xar_t x, err_handler callback, void *usrctx);
xar_t xar_err_get_archive(xar_errctx_t ctx);
xar_file_t xar_err_get_file(xar_errctx_t ctx);
const char *xar_err_get_string(xar_errctx_t ctx);
int xar_err_get_errno(xar_errctx_t ctx);
void  xar_err_set_file(xar_t x, xar_file_t f);
void  xar_err_set_string(xar_t x, const char *str);
void  xar_err_set_errno(xar_t x, int e);
void xar_err_new(xar_t x);
int32_t xar_err_callback(xar_t x, int32_t sev, int32_t err);

void xar_serialize(xar_t x, const char *file);
char *xar_get_path(xar_file_t f);
uint64_t xar_get_heap_offset(xar_t x);
uint64_t xar_ntoh64(uint64_t num);

#ifdef __cplusplus
}
#endif

#endif /* _XAR_H_ */
