use std::io::{self, Read};

pub fn round_align(n: u64, align: u64) -> u64 {
    ((n + align - 1) / align) * align
}

pub fn string_from_bytes(buf: &[u8]) -> String {
    let end = buf.iter().position(|&b| b == 0).unwrap_or(buf.len());
    String::from_utf8_lossy(&buf[..end]).to_string()
}

pub fn read_exact<R: Read>(reader: &mut R, size: usize) -> io::Result<Vec<u8>> {
    let mut buf = vec![0u8; size];
    reader.read_exact(&mut buf)?;
    Ok(buf)
}