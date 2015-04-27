#!/usr/bin/env ruby
require 'bundler'
require 'mithril'
input = ARGV.shift
symname = ARGV.shift
outfile = ARGV.shift
file = Elf::Parser.from_file(input)
sym = file.symbols[symname]
data = sym.section.data.bytes.to_a[sym.sectoffset .. sym.sectoffset + sym.size]
File.open(outfile, mode ="w") do |f|
  f.write(data.pack("C*"))
end
#print(data)
