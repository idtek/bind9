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

#include <config.h>

#include <stddef.h>
#include <string.h>

#include <isc/assertions.h>

#include <dns/rdata.h>
#include <dns/rdataclass.h>
#include <dns/rdatatype.h>
#include <dns/rdataset.h>

void
dns_rdataset_init(dns_rdataset_t *rdataset) {

	/*
	 * Make 'rdataset' a valid, disassociated rdataset.
	 */

	REQUIRE(rdataset != NULL);

	rdataset->magic = DNS_RDATASET_MAGIC;
	rdataset->methods = NULL;
	ISC_LINK_INIT(rdataset, link);
	rdataset->class = 0;
	rdataset->type = 0;
	rdataset->ttl = 0;
	rdataset->private1 = NULL;
	rdataset->private2 = NULL;
	rdataset->private3 = NULL;
}

void
dns_rdataset_invalidate(dns_rdataset_t *rdataset) {

	/*
	 * Invalidate 'rdataset'.
	 */

	REQUIRE(DNS_RDATASET_VALID(rdataset));
	REQUIRE(rdataset->methods == NULL);
	
	rdataset->magic = 0;
	ISC_LINK_INIT(rdataset, link);
	rdataset->class = 0;
	rdataset->type = 0;
	rdataset->ttl = 0;
	rdataset->private1 = NULL;
	rdataset->private2 = NULL;
	rdataset->private3 = NULL;
}

void
dns_rdataset_disassociate(dns_rdataset_t *rdataset) {

	/*
	 * Disassocate 'rdataset' from its rdata, allowing it to be reused.
	 */

	REQUIRE(DNS_RDATASET_VALID(rdataset));

	(rdataset->methods->disassociate)(rdataset);
	rdataset->methods = NULL;
	ISC_LINK_INIT(rdataset, link);
	rdataset->class = 0;
	rdataset->type = 0;
	rdataset->ttl = 0;
	rdataset->private1 = NULL;
	rdataset->private2 = NULL;
	rdataset->private3 = NULL;
}

dns_result_t
dns_rdataset_first(dns_rdataset_t *rdataset) {

	/*
	 * Move the rdata cursor to the first rdata in the rdataset (if any).
	 */

	REQUIRE(DNS_RDATASET_VALID(rdataset));

	return ((rdataset->methods->first)(rdataset));
}

dns_result_t
dns_rdataset_next(dns_rdataset_t *rdataset) {

	/*
	 * Move the rdata cursor to the next rdata in the rdataset (if any).
	 */

	REQUIRE(DNS_RDATASET_VALID(rdataset));

	return ((rdataset->methods->next)(rdataset));
}

void
dns_rdataset_current(dns_rdataset_t *rdataset, dns_rdata_t *rdata) {

	/*
	 * Make 'rdata' refer to the current rdata.
	 */

	REQUIRE(DNS_RDATASET_VALID(rdataset));

	(rdataset->methods->current)(rdataset, rdata);
}

static char *tabs = "\t\t\t\t\t\t\t\t\t\t";

static inline int
tabs_needed(unsigned int current_offset, unsigned int desired_offset) {
	unsigned int needed;
	unsigned int spaces;

	/*
	 * Assumes tabs are 8 characters.
	 */

	if (current_offset >= desired_offset)
		return (1);
	spaces = desired_offset - current_offset;
	needed = spaces / 8;
	if (spaces % 8 != 0)
		needed++;
	if (needed > 10)
		needed = 10;
	return (needed);
}

dns_result_t
dns_rdataset_totext(dns_rdataset_t *rdataset,
		    dns_name_t *owner_name,
		    isc_boolean_t omit_final_dot,
		    isc_buffer_t *target)
{
	dns_result_t result;
	unsigned int common_start, common_length, length, ntabs, ttabs;
	char *common;
	dns_rdata_t rdata;
	isc_boolean_t first = ISC_TRUE;
	isc_region_t r;
	char ttl[64];

	/*
	 * Convert 'rdataset' to text format, storing the result in 'target'.
	 */

	REQUIRE(DNS_RDATASET_VALID(rdataset));
	result = dns_rdataset_first(rdataset);
	REQUIRE(result == DNS_R_SUCCESS);

	/*
	 * XXX Explicit buffer structure references here.  Improve buffer
	 * API.
	 */
	common_start = target->used;
	/*
	 * The caller might want to give us an empty owner
	 * name (e.g. if they are outputting into a master
	 * file and this rdataset has the same name as the
	 * previous one.)
	 */
	if (dns_name_countlabels(owner_name) != 0) {
		result = dns_name_totext(owner_name,
					 omit_final_dot,
					 target);
		if (result != DNS_R_SUCCESS)
			return (result);
	}
	common = (char *)target->base + common_start;
	common_length = target->used - common_start;
	ntabs = tabs_needed(common_length, 24);
	ttabs = ntabs;
	isc_buffer_available(target, &r);
	if (r.length < ntabs)
		return (DNS_R_NOSPACE);
	memcpy(r.base, tabs, ntabs);
	isc_buffer_add(target, ntabs);
	/*
	 * XXX The following sprintf() is safe, but it
	 * would still be good to use snprintf if we had it.
	 */
	length = sprintf(ttl, "%u ", rdataset->ttl);
	INSIST(length <= sizeof ttl);
	isc_buffer_available(target, &r);
	if (r.length < length)
		return (DNS_R_NOSPACE);
	memcpy(r.base, ttl, length);
	isc_buffer_add(target, length);
	result = dns_rdataclass_totext(rdataset->class, target);
	if (result != DNS_R_SUCCESS)
		return (result);
	isc_buffer_available(target, &r);
	if (r.length == 0)
		return (DNS_R_NOSPACE);
	*r.base = ' ';
	isc_buffer_add(target, 1);
	result = dns_rdatatype_totext(rdataset->type, target);
	if (result != DNS_R_SUCCESS)
		return (result);
	common_length = target->used - common_start;
	ntabs = tabs_needed(common_length + ttabs * 7, 40);
	ttabs += ntabs;
	isc_buffer_available(target, &r);
	if (r.length < ntabs)
		return (DNS_R_NOSPACE);
	memcpy(r.base, tabs, ntabs);
	isc_buffer_add(target, ntabs);
	common_length = target->used - common_start;

	do {
		if (!first) {
			isc_buffer_available(target, &r);
			if (r.length < common_length)
				return (DNS_R_NOSPACE);
			memcpy(r.base, common, common_length);
			isc_buffer_add(target, common_length);
		} else
			first = ISC_FALSE;

		dns_rdataset_current(rdataset, &rdata);
		result = dns_rdata_totext(&rdata, target);
		if (result != DNS_R_SUCCESS)
			return (result);
		isc_buffer_available(target, &r);
		if (r.length < 1)
			return (DNS_R_NOSPACE);
		memcpy(r.base, "\n", 1);
		isc_buffer_add(target, 1);

		result = dns_rdataset_next(rdataset);
	} while (result == DNS_R_SUCCESS);

	if (result != DNS_R_NOMORE)
		return (result);

	return (DNS_R_SUCCESS);
}

dns_result_t
dns_rdataset_towire(dns_rdataset_t *rdataset,
		    dns_name_t *owner_name,
		    dns_compress_t *cctx,
		    isc_buffer_t *target)
{
	dns_rdata_t rdata;
	isc_region_t r;
	dns_rdataclass_t rclass;
	dns_rdatatype_t rtype;
	dns_ttl_t rttl;
	dns_result_t result;

	/*
	 * Convert 'rdataset' to wire format, compressing names as specified
	 * in cctx, and storing the result in 'target'.
	 */

	REQUIRE(DNS_RDATASET_VALID(rdataset));
	result = dns_rdataset_first(rdataset);
	REQUIRE(result == DNS_R_SUCCESS);

	do {
		/*
		 * copy out the name, type, class, ttl.
		 */
		result = dns_name_towire(owner_name, cctx, target);
		if (result != DNS_R_SUCCESS)
			return (result);
		isc_buffer_available(target, &r);
		if (r.length < (sizeof(dns_rdataclass_t)
				+ sizeof(dns_rdatatype_t)
				+ sizeof(dns_ttl_t)
				+ 2)) /* XXX 2? it's for the rdata length */
			return (DNS_R_NOSPACE);
		rtype = rdataset->type;
		r.base[0] = (rtype & 0xff00) >> 8;
		r.base[1] = (rtype & 0xff);
		rclass = rdataset->class;
		r.base[2] = (rclass & 0xff00) >> 8;
		r.base[3] = (rclass & 0x00ff);
		rttl = rdataset->ttl;
		r.base[4] = (rttl & 0xff000000) >> 24;
		r.base[5] = (rttl & 0x00ff0000) >> 16;
		r.base[6] = (rttl & 0x0000ff00) >> 8;
		r.base[7] = (rttl & 0x000000ff);
		isc_buffer_add(target, (sizeof(dns_rdataclass_t)
					+ sizeof(dns_rdatatype_t)
					+ sizeof(dns_ttl_t)
					+ 2)); /* XXX see XXX above */
		/*
		 * copy out the rdata length
		 */
		dns_rdataset_current(rdataset, &rdata);
		r.base[8] = (rdata.length & 0xff00) >> 8;
		r.base[9] = (rdata.length & 0x00ff);

		/*
		 * copy out the rdata
		 */
		result = dns_rdata_towire(&rdata, cctx, target);
		if (result != DNS_R_SUCCESS)
			return (result);

		result = dns_rdataset_next(rdataset);
	} while (result == DNS_R_SUCCESS);

	if (result != DNS_R_NOMORE)
		return (result);

	return (DNS_R_SUCCESS);
}
