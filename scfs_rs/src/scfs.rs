use binrw::{BinReaderExt, Endian};
use crate::util::{read_exact, round_align, string_from_bytes};
use std::io::{Read, Seek, SeekFrom};

use crate::include::*;

pub struct Scfs {
    pub superblock: ScfsSuperBlock,
    pub enc_param: Vec<u8>,
    pub comp_param: Vec<u8>,
}
impl Scfs {
    pub fn open<T: Read + Seek>(reader: &mut T, endian: Endian) -> Result<Self, Box<dyn std::error::Error>> {

        //read superblock
        let superblock: ScfsSuperBlock = reader.read_type(endian)?;

        //check magic value
        if superblock.magic != SCFS_MAGIC {
            return Err("invalid SCFS magic value".into());
        }
        if superblock.version != SCFS_VERSION{
            return Err("unsupported SCFS version".into());
        }
        if superblock.scfs_super_size != SCFS_SUPER_SIZE {
            return Err("invalid SCFS superblock size".into());
        }

        //read enc and comp param, appears directly after superblock
        let enc_param = read_exact(reader, superblock.enc_param_size as usize)?;
        let comp_param = read_exact(reader, superblock.comp_param_size as usize)?;

        //seek to data start
        reader.seek(SeekFrom::Start(superblock.super_total_size.into()))?;

        //  start reading the metadata (inodes)
        if superblock.is_meta_encrypted() {
            return Err("metadata is encrypted".into());
        }

        //read the root inode
        let root_ino: CommonInode = reader.read_type(endian)?;

        if root_ino.inode_type() != InodeType::Directory {
            return Err("root inode is not a directory".into());
        }

        Ok(Scfs {
            superblock,
            enc_param,
            comp_param
        })
    }

    fn read_inode<T: Read + Seek>(reader: &mut T, endian: Endian, offset: u32, size: u32) -> Result<MInode, Box<dyn std::error::Error>> {
        let ini_pos = reader.stream_position()?;
        let mut read = 0;

        reader.seek(SeekFrom::Start(offset.into()));

        //read common inode header
        let common: CommonInode = reader.read_type(endian)?;
        read += 16;

        let inode: Inode = match common.inode_type() {
            InodeType::RegularFile => {
                let i_inode: RegularFileInode = reader.read_type(endian)?;
                read += 20;
                Inode::RegularFile(i_inode)
            },
            InodeType::Directory => {
                let i_inode: DirectoryInode = reader.read_type(endian)?;
                read += 16;
                Inode::Directory(i_inode)
            },
            InodeType::Symlink => {
                let i_inode: SymlinkInode = reader.read_type(endian)?;
                read += 4;
                Inode::Symlink(i_inode)
            },
            InodeType::CharDevice => {
                let i_inode: DeviceInode = reader.read_type(endian)?;
                read += 4;
                Inode::CharDevice(i_inode)
            },
            InodeType::BlockDevice => {
                let i_inode: DeviceInode = reader.read_type(endian)?;
                read += 4;
                Inode::BlockDevice(i_inode)
            },
            InodeType::FIFO => {
                let i_inode: SpecialInode = reader.read_type(endian)?;
                Inode::Fifo(i_inode)
            },
            InodeType::Socket => {
                let i_inode: SpecialInode = reader.read_type(endian)?;
                Inode::Fifo(i_inode)
            },
        };

        //read name
        let pad_name_size = round_align(common.name_len as u64 +1 , 4);
        let name_bytes = read_exact(reader, pad_name_size as usize)?;
        read += pad_name_size;

        let name = string_from_bytes(&name_bytes);

        Ok( MInode {
            common,
            inode,
            name,
            total_size: read as usize,
            children: vec![]
        })

    }

}