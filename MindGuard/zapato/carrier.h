//
//  carrier.h
//  MGModule
//
//  Created by Richard Henry on 17/06/2021.
//

#ifndef carrier_h
#define carrier_h

#ifdef __cplusplus
extern "C" {
#endif

void init_mods(const char *mg_path);

const char *mg_version = "0.0.0.4";
const char *mg_year = "2003";

void dump_carriers();

#ifdef __cplusplus
}
#endif

#endif /* carrier_h */
