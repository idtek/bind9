/*
 * Copyright (C) 1998, 1999  Internet Software Consortium.
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

 /* $Id: hinfo_13.c,v 1.17 1999/09/15 23:03:29 explorer Exp $ */

#ifndef RDATA_GENERIC_HINFO_13_C
#define RDATA_GENERIC_HINFO_13_C

static inline dns_result_t
fromtext_hinfo(dns_rdataclass_t rdclass, dns_rdatatype_t type,
	       isc_lex_t *lexer, dns_name_t *origin,
	       isc_boolean_t downcase, isc_buffer_t *target)
{
	isc_token_t token;
	int i;

	REQUIRE(type == 13);

	rdclass = rdclass;		/*unused*/
	origin = origin;	/*unused*/
	downcase = downcase;	/*unused*/

	for (i = 0; i < 2 ; i++) {
		RETERR(gettoken(lexer, &token, isc_tokentype_qstring,
				ISC_FALSE));
		RETERR(txt_fromtext(&token.value.as_textregion, target));
	}
	return (DNS_R_SUCCESS);
}

static inline dns_result_t
totext_hinfo(dns_rdata_t *rdata, dns_rdata_textctx_t *tctx, 
	     isc_buffer_t *target) 
{
	isc_region_t region;

	REQUIRE(rdata->type == 13);

	tctx = tctx;	/*unused*/

	dns_rdata_toregion(rdata, &region);
	RETERR(txt_totext(&region, target));
	RETERR(str_totext(" ", target));
	return (txt_totext(&region, target));
}

static inline dns_result_t
fromwire_hinfo(dns_rdataclass_t rdclass, dns_rdatatype_t type,
	       isc_buffer_t *source, dns_decompress_t *dctx,
	       isc_boolean_t downcase, isc_buffer_t *target)
{

	REQUIRE(type == 13);

	dctx = dctx;		/* unused */
	rdclass = rdclass;		/* unused */
	downcase = downcase;	/* unused */

	RETERR(txt_fromwire(source, target));
	return (txt_fromwire(source, target));
}

static inline dns_result_t
towire_hinfo(dns_rdata_t *rdata, dns_compress_t *cctx, isc_buffer_t *target) {

	REQUIRE(rdata->type == 13);

	cctx = cctx;	/*unused*/

	return (mem_tobuffer(target, rdata->data, rdata->length));
}

static inline int
compare_hinfo(dns_rdata_t *rdata1, dns_rdata_t *rdata2) {
	isc_region_t r1;
	isc_region_t r2;
	
	REQUIRE(rdata1->type == rdata2->type);
	REQUIRE(rdata1->rdclass == rdata2->rdclass);
	REQUIRE(rdata1->type == 13);

	dns_rdata_toregion(rdata1, &r1);
	dns_rdata_toregion(rdata2, &r2);
	return (compare_region(&r1, &r2));
}

static inline dns_result_t
fromstruct_hinfo(dns_rdataclass_t rdclass, dns_rdatatype_t type, void *source,
		 isc_buffer_t *target)
{

	REQUIRE(type == 13);

	rdclass = rdclass;	/*unused*/

	source = source;
	target = target;

	return (DNS_R_NOTIMPLEMENTED);
}

static inline dns_result_t
tostruct_hinfo(dns_rdata_t *rdata, void *target, isc_mem_t *mctx) {

	REQUIRE(rdata->type == 13);

	target = target;
	mctx = mctx;

	return (DNS_R_NOTIMPLEMENTED);
}

static inline void
freestruct_hinfo(void *source) {
	REQUIRE(source != NULL);
	REQUIRE(ISC_FALSE); /* XXX */
}

static inline dns_result_t
additionaldata_hinfo(dns_rdata_t *rdata, dns_additionaldatafunc_t add,
		     void *arg)
{
	REQUIRE(rdata->type == 13);

	(void)add;
	(void)arg;

	return (DNS_R_SUCCESS);
}

static inline dns_result_t
digest_hinfo(dns_rdata_t *rdata, dns_digestfunc_t digest, void *arg) {
	isc_region_t r;

	REQUIRE(rdata->type == 13);

	dns_rdata_toregion(rdata, &r);

	return ((digest)(arg, &r));
}

#endif	/* RDATA_GENERIC_HINFO_13_C */
