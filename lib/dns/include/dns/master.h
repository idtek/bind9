/*
 * Copyright (C) 1999  Internet Software Consortium.
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#ifndef DNS_MASTER_H
#define DNS_MASTER_H 1

/***
 ***	Imports
 ***/

#include <isc/lang.h>
#include <isc/mem.h>
#include <isc/lex.h>

#include <dns/types.h>
#include <dns/result.h>
#include <dns/name.h>
#include <dns/rdataset.h>
#include <dns/callbacks.h>

ISC_LANG_BEGINDECLS

/***
 ***	Function
 ***/

dns_result_t dns_master_load(char *master_file,
			     dns_name_t *top,
			     dns_name_t *origin,
			     dns_rdataclass_t class,
			     int *soacount,
			     int *nscount,
			     dns_rdatacallbacks_t *callbacks,
			     isc_mem_t *mctx);

/*
 * Loads a RFC 1305 master file from disk into rdatasets then call
 * 'callbacks->commit' to commit the dataset.  Rdata memory belongs
 * to dns_master_load and will be reused / released when the callback
 * completes.  dns_load_master will abort if callbacks->commit returns
 * any value other than DNS_R_SUCCESS.
 *
 * 'callbacks->commit' is assumed to call 'callbacks->error' or
 * 'callbacks->warn' to generate any error messages required.
 *
 * Requires:
 *	'master_file' to point to a valid string.
 *	'top' to point to a valid name.
 *	'origin' to point to a valid name.
 *	'soacount' to point to a int.
 *	'nscount' to point to a int.
 *	'callbacks->commit' to point ta a valid function.
 *	'callbacks->error' to point ta a valid function.
 *	'callbacks->warn' to point ta a valid function.
 *	'mctx' to point to a memory context.
 *
 * Returns:
 *	DNS_R_SUCCESS upon successfully loading the master file.
 *	DNS_R_NOMEMORY out of memory.
 *	DNS_R_UNEXPECTEDEND expected to be able to read a input token and
 *		there was not one.
 *	DNS_R_UNEXPECTED
 *	DNS_R_NOOWNER failed to specify a ownername.
 *	DNS_R_NOTTL failed to specify a ttl.
 *	DNS_R_BADCLASS record class did not match zone class.
 *	Any dns_rdata_fromtext() error code.
 *	Any error code from callbacks->commit().
 */

ISC_LANG_ENDDECLS

#endif	/* DNS_MASTER_H */
