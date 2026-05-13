# SCFS - general

SCFS is a read-only filesystem developed by Sony, initially sometime around 2007-2008.   
It is used in some of their Linux-based Blu-ray Players and recorders, primarily ones to be used in Japan.  
The actual meaning of the name is not known, but its probably someting like Secure Compressed File System.

It is used to store an encrypted image of the rootfs, and also for distributing user software upgrades.

Two versions of the filesystem are known - 1.1, and 1.3, which seems to be the last version.

The filesystem primarily revolves around the use of two types of plugins - a compression plugin and an encryption plugin. When creating the filesytem both have to be specified, even if compression and encryption are not used.

## Compression
There are two known compression options for SCFS - LZO and zlib.
Zlib seems to be used only for the firmware packages, while LZO is used for the on-device filesystems. The device itself can decompress both types, but create only LZO.    
Compression is not used for metadata, only for actual file data.

## Encryption
Two encryption plugins can be observed in SCFS images. One of them is known and it is `emma3r` encryption, with ID 0x80050100. It communicates directly with some hardware encryption layer on `/dev/emma_crypt`.   
A different encryption plugin can be seen on older images, with ID: 0x80040100. While the exact name and operation is not known, it can be assumed that it is an older version of the above one, probably meant for different SoC. It can be also noted that this one uses alignment to an 8 byte block boundary, unlike the known `emma3r` method, which aligns to 16 bytes.   
Both the actual data and metadata can be encrypted in SCFS.


