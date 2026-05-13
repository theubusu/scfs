#![allow(dead_code)]
use binrw::{BinRead};
use crate::util::{string_from_bytes};

pub static SCFS_MAGIC: u32 = 0x73636673;
pub static SCFS_VERSION: u16 = 0x0103;
pub static SCFS_SUPER_SIZE: u32 = 0xBC;

#[derive(BinRead, Debug)]
pub struct ScfsSuperBlock {
    pub magic: u32,
    pub label: [u8; 16],
    pub version: u16,
    pub data_blksize_shift: u8,
    pub meta_blksize_shift: u8,
    pub flags: u32,
    pub timestamp: u32,
    pub comp_plugin_id: u32,
    pub enc_plugin_id: u32,
    pub scfs_super_size: u32,
    pub enc_param_size: u32,
    pub comp_param_size: u32,
    pub uid_table: [u32; 16],
    pub gid_table: [u32; 16],
    pub super_total_size: u32,
    pub comp_blksize_shift: u8,
    _pad: [u8; 3],
}
impl ScfsSuperBlock {
    pub fn label_string(&self) -> String {
        string_from_bytes(&self.label)
    }

    //blksize
    pub fn data_blksize(&self) -> usize {
        1 << self.data_blksize_shift
    }
    pub fn meta_blksize(&self) -> usize {
        1 << self.meta_blksize_shift
    }
    pub fn comp_blksize(&self) -> usize {
        1 << self.comp_blksize_shift
    }
    
    //flags
    pub fn is_big_endian(&self) -> bool {
        (self.flags & (1 << 0)) != 0
    }
    pub fn is_data_encrypted(&self) -> bool {
        (self.flags & (1 << 1)) != 0
    }
    pub fn is_data_compressed(&self) -> bool {
        (self.flags & (1 << 2)) != 0
    }
    pub fn is_meta_encrypted(&self) -> bool {
        (self.flags & (1 << 4)) != 0
    }
}

#[derive(BinRead, Debug)]
pub struct CommonInode {
    pub type_uid_gid_perm: u32,
    pub inode_size: u32,
    pub unk1: u32,
    pub version: u16,
    pub name_len: u8,
    pub unk2: u8,
}
impl CommonInode {
    pub fn inode_type(&self) -> InodeType {
        InodeType::from_int(self.type_uid_gid_perm >> 0x18)
    }
    pub fn uid_idx(&self) -> u32 {
        (self.type_uid_gid_perm & 0x00F00000) >> 20
    }
    pub fn gid_idx(&self) -> u32 {
        (self.type_uid_gid_perm & 0x000F0000) >> 16
    }
    pub fn perms(&self) -> u32 {
        (self.type_uid_gid_perm >> 8) & 0x1FF
    }
}

//PARSED INODE COMMON STRUCT
pub struct MInode {
    pub common: CommonInode,
    pub inode: Inode,
    pub name: String,
    pub total_size: usize,

    pub children: Vec<MInode>,  //???
}

pub enum Inode {
    RegularFile(RegularFileInode),
    Directory(DirectoryInode),
    Symlink(SymlinkInode),
    CharDevice(DeviceInode),
    BlockDevice(DeviceInode),
    Fifo(SpecialInode),
    Socket(SpecialInode),
}

#[derive(BinRead, Debug)]
pub struct RegularFileInode {
    pub timestamp: u32,
    pub compressed_size: u32,
    pub uncompressed_size: u32,
    pub data_offset: u32,
    pub unk: u32,       //purpose unknown
}

#[derive(BinRead, Debug)]
pub struct DirectoryInode {
    pub timestamp: u32,
    pub size: u32,      //of all inodes in the directory
    pub offset: u32,    //of the first inode in the directory
    pub unk: u32,       //unknown purpose
}


#[derive(BinRead, Debug)]
pub struct SymlinkInode {
    pub dest_len: u32,
}

#[derive(BinRead, Debug)]   //for chardevice, blockdevice
pub struct DeviceInode {
    pub un1: u16,
    pub ids_or_smth: u16,
}

#[derive(BinRead, Debug)]   //for fifo, socket. it is EMPTY?
pub struct SpecialInode {

}

#[derive(Debug, PartialEq)]
pub enum InodeType {
    RegularFile = 0,
    Directory,
    Symlink,
    CharDevice,
    BlockDevice,
    FIFO,
    Socket
}
impl InodeType {
    pub fn from_int(val: u32) -> InodeType {
        match val {
            0 => InodeType::RegularFile,
            1 => InodeType::Directory,
            2 => InodeType::Symlink,
            3 => InodeType::CharDevice,
            4 => InodeType::BlockDevice,
            5 => InodeType::FIFO,
            6 => InodeType::Socket,
            _ => panic!("invalid inode type int")
        }
    }
}