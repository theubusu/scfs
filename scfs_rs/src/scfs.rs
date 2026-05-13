use binrw::{BinRead, BinReaderExt};
use crate::util::{string_from_bytes, read_exact};
use std::io::{Read, Seek, SeekFrom};

static SCFS_MAGIC: u32 = 0x73636673;

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

pub struct Scfs {
    pub superblock: ScfsSuperBlock,
    pub enc_param: Vec<u8>,
    pub comp_param: Vec<u8>,
}
impl Scfs {
    pub fn open<T: Read + Seek>(reader: &mut T) -> Result<Self, Box<dyn std::error::Error>> {

        //read superblock
        let superblock: ScfsSuperBlock = reader.read_be()?;

        //check magic value
        if superblock.magic != SCFS_MAGIC {
            return Err("invalid SCFS magic value".into());
        }
        if superblock.version != 0x0103 {
            return Err("unsupported SCFS version".into());
        }
        if superblock.scfs_super_size != 0xBC {
            return Err("invalid SCFS superblock size".into());
        }

        //read enc and comp param, appears directly after superblock
        let enc_param = read_exact(reader, superblock.enc_param_size as usize)?;
        let comp_param = read_exact(reader, superblock.comp_param_size as usize)?;

        //seek to data start
        reader.seek(SeekFrom::Start(superblock.super_total_size.into()))?;

        Ok(Scfs {
            superblock,
            enc_param,
            comp_param
        })
    }
}