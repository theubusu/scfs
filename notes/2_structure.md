# SCFS - Structure

## Superblock

The SCFS superblock appears at the beginning of the image, and its size is always 188(0xBC) bytes. Its structure is as follows:

| Type    | Name               | Description                                                                         |
|---------|--------------------|-------------------------------------------------------------------------------------|
| u32     | magic              | always 0x73636673                                                                   |
| u8[16]  | label              | hardcoded "scfs" in mkscfs, with no given way to change it                          |
| u16     | version            | major\|minor                                                                        |
| u8      | data_blksize_shift | data block size = 1 << data_blksize_shift                                           |
| u8      | meta_blksize_shift | metadata block size = 1 << meta_blksize_shift                                       |
| u32     | flags              | bitmap - 0=endian(0=little, 1=big), 1=data_enc, 2=data_comp, 4=meta_enc             |
| u32     | timestamp          | unix timestamp of filesystem creation, can be set by mkscfs flag                    |
| u32     | comp_plugin_id     | ID of the used compression plugin                                                   |
| u32     | enc_plugin_id      | ID of the used encryption plugin                                                    |
| u32     | scfs_super_size    | size of SCFS superblock (0xBC set)                                                  |
| u32     | enc_param_size     | size of encryption params                                                           |
| u32     | comp_param_size    | size of compression params                                                          |
| u32[16] | uid_table          | UID substitution table                                                              |
| u32[16] | gid_table          | GID substitution table                                                              |
| u32     | super_total_size   | the total superblock size (including enc/comp param and padding), where data starts |
| u8      | comp_blksize_shift | compressed data block size = 1 << comp_blksize_shift                                |
| u8[3]   | pad                | padding, not used (0x00)                                                            |

After the SCFS superblock, optional encryption and compression params follow. Their structure is decided and verified by the respective plugin.    
The superblock can be additionally aligned based on the specification of encryption plugin. The padding data is genereated by rand(), with optional seed that can be provided in `mkscfs`.

## Note about UID/GID
SCFS can contain a total of 16 UIDs and 16 GIDs, stored in the superblock. Each inode then stores an index into each table.

## Inodes

SCFS supports a total of 7 inode types, as follows:

| SCFS inode type | Inode size | POSIX type   |
|-----------------|------------|--------------|
| 0               | 0x24       | regular file |
| 1               | 0x20       | directory    |
| 2               | 0x14       | symlink      |
| 3               | 0x14       | char device  |
| 4               | 0x14       | block device |
| 5               | 0x10       | FIFO         |
| 6               | 0x10       | socket       |

Every inode is preceeded by the Common inode header of 16 bytes, with the following structure:

| Type | Name          | Description                                                                                   |
|------|---------------|-----------------------------------------------------------------------------------------------|
| u32  | type_uid_guid | type = this >> 0x18, uid_idx = (this & 0x00F00000) >> 20, gid_idx = (this & 0x000F0000) >> 16 |
| u32  | inode_size    | size of the inode, used as the offset of the name                                             |
| u32  | unk1          | not known yet                                                                                 |
| u16  | version       | version of SCFS, major\|minor                                                                 |
| u8   | name_len      | lenght of the name                                                                            |
| u8   | unk2          | not known yet                                                                                 |

Then, each inode has its own data structure following it, as follows for:

### 0 - Regular file
Size: 20
| Type | Name              | Description                     |
|------|-------------------|---------------------------------|
| u32  | timestamp         | file timestamp                  |
| u32  | compressed_size   | TO BE VERIFIED                  |
| u32  | uncompressed_size | TO BE VERIFIED                  |
| u32  | data_offset       | offset of the data in the image |
| u32  | unk               | always empty?                   |

### 1 - Directory
Size: 16
| Type | Name      | Description                         |
|------|-----------|-------------------------------------|
| u32  | timestamp | dir timestamp                       |
| u32  | size      | of all inodes in the directory      |
| u32  | offset    | of the first inode in the directory |
| u32  | unk       | not known yet                       |


### 2 - Symlink
Size: 4
| Type | Name     | Description                    |
|------|----------|--------------------------------|
| u32  | dest_len | lenght of the destination path |

The destination path appears directly after the inode's name. It is aligned to 4byte boundary, and NOT null-terminated. 

### 3 - Char device
TBD

### 4 - Block device
TBD

### 5 - FIFO
There is no inode data for this type.

### 6 - Socket
There is no inode data for this type.

After the type-specific section, the name is stored. It is null-terminated, and aligned to 4 byte bonundary. Both of these are not included in the size, so they need to be calculated manually!