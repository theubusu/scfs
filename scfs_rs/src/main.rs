mod scfs;
mod include;
mod util;

use binrw::Endian;
use clap::Parser;
use chrono::DateTime;
use std::fs::{File};

use scfs::Scfs;

#[derive(Parser, Debug)]
struct Args {
    #[arg(short = 'l')]
    little_endian: bool,

    input_file: String,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args = Args::parse();

    let in_file = args.input_file;
    println!("Input file: {}", in_file);
    let mut file = File::open(in_file)?;

    let endianness = match args.little_endian { //assume BE by default, since its the most common used in SCFS
        true => Endian::Little,
        false => Endian::Big
    };

    //open fs
    let scfs = Scfs::open(&mut file, endianness)?;

    println!("SCFS open OK!");
    println!("superblock info -\nLabel: {}\nCreation time: {}\nCompression plugin ID: {:x}\nEncryption plugin ID: {:x}\nData blocksize: {}\nMeta blocksize: {}\nComp blocksize: {}\nTotal superblock size: {}", 
        scfs.superblock.label_string(), 
        DateTime::from_timestamp(scfs.superblock.timestamp as i64, 0).unwrap(),
        scfs.superblock.comp_plugin_id,
        scfs.superblock.enc_plugin_id,
        scfs.superblock.data_blksize(),
        scfs.superblock.meta_blksize(),
        scfs.superblock.comp_blksize(),
        scfs.superblock.super_total_size
    );

    println!("Is big endian: {}\nIs data encrypted: {}\nIs data compressed: {}\nIs meta encrypted: {}",
        scfs.superblock.is_big_endian(),
        scfs.superblock.is_data_encrypted(),
        scfs.superblock.is_data_compressed(),
        scfs.superblock.is_meta_encrypted(),
    );

    Ok(())
}
