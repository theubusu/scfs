import struct
import sys

def round_up(n, multiple):
    return ((n + multiple - 1) // multiple) * multiple

ino_i = 0
with open(sys.argv[1], "rb") as file:
    #skip root inode
    file.seek(0xC0 + 0x20)
    
    while True:
        
        #common inode 
        type_uid_guid = struct.unpack('>I', file.read(4))[0]
        inode_size = struct.unpack('>I', file.read(4))[0]
        unk = struct.unpack('>I', file.read(4))[0]
        version = file.read(2)
        name_len = struct.unpack('>B', file.read(1))[0]
        pad = file.read(1)
        
        inode_type = type_uid_guid >> 0x18
        uid_idx = (type_uid_guid & 0x00F00000) >> 20
        gid_idx = (type_uid_guid & 0x000F0000) >> 16
        perms = (type_uid_guid >> 8) & 0x1FF
        
        print(f"INODE {ino_i+1}, type: {inode_type}, size: {inode_size}, u/g:{uid_idx}/{gid_idx}, p:{oct(perms)}, name_len: {name_len}, pad: {pad}, unk: {unk}")
        
        ino_d = file.read(inode_size - 16)
        
        if not name_len == 0:
            new_namelen = round_up(name_len+1, 4)
        
            name = file.read(new_namelen).split(b'\x00', 1)[0].decode('utf-8')
            print(f"- name: {name}, newlen: {new_namelen}")
            
        if inode_type == 2:
            sym_org = struct.unpack('>I', ino_d)[0]
            sym_len = round_up(sym_org, 4)
            print(f"sym org: {sym_org}")
            print(f"sym len: {sym_len}")
            print(f"rea off: {file.tell()}")
            tgt = file.read(sym_len)
        
        ino_i += 1
        #break