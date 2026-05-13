import struct

raw = b"\x01\x03\x10\x10"
b0, b1, b2, b3 = raw

ver_hi, ver_lo = b0, b1
data_log = b2
meta_log = b3

data_bs = 1 << data_log
#comp_bs = 1 << comp_log
meta_bs = 1 << meta_log

print

print({
    'version':     (ver_hi, ver_lo), 
    'data_bs':     data_bs,
    #'comp_bs':     comp_bs,
    'meta_bs':     meta_bs,
    'data_log':    data_log,
    #'comp_log':    comp_log,
    'meta_log':    meta_log,
})