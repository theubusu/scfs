# SCFS - resources

## Binaries

Most important resources about SCFS are the related binaries dumped from the filesystem of a device utilizing it.  
Currently, the following are avaliable (All are for MIPS-BE architecture):
- `mkscfs` (v1.3) - this is the tool that allows to create the actual filesystem image from a specified directory 
- `libscfs_lzo.so` - this is the library that makes `mkscfs` be able to compress the filesystem with LZO. It interestingly uses some device LZO compressor at `/dev/lzo`, so it cannot be used just yet.
- `libscfs_emma3r.so` - this allows `mkscfs` to encrypt the filesystem with `emma3r` encryption. As mentioned in previous section, this utilizes hardware encryption.
- `scfs.ko` (v1.3) - kernel driver for SCFS
- `scfs_uncompression_lzo.ko` - kernel driver for LZO decompression
- `scfs_uncompression_zlib.ko` - kernel driver for zlib decompression
- `scfs_decryption_emma3r.ko` - kernel driver for `emma3r` decryption

## GPL Source

Sony has shared on its open-source releases site (https://oss.sony.net/Products/Linux/common/search.html), the source code for the LZO decompression plugin for SCFS. It contains useful code describing SCFS plugin structure.  
One of those releases is avaliable in `scfs-gpl-src` directory in this repository

## Images

Because we have `mkscfs`, images can be created as we wish. However, they cannot be encrypted due to the use of hardware-based crypto. LZO and zlib compressors also haven't been made working yet, but that is something that can be recreated.
Encrypted images can be found in firmware packages released by sony (Useful list: https://gist.github.com/uyjulian/b0f34ded4f074f743390eae212fdc6ca), or dumped from devices. But again, they cannot be decrypted (unless you have access to a device with the respective SoC)