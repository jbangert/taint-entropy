import struct
class FileRecord:
    FMT = struct.Struct("QiB")
    BEGIN = 0
    READ = 2
    WRITE = 1
    END = 3
    def __init__(self, file):
        s = file.read(FMT.size)
        self.addr, self.type, self.value = FMT.unpack(s)
        
class PossibleOperation:
    def __init__(self, file):
        while True:
            x = FileRecord(file)
            if(x.type == FileRecord.BEGIN):
                break
        writes ={}
        reads = {}
        while True:
            x = FileRecord(file)
            if(x.type == FileRecord.END):
                break
            if(x.type == FileRecord.WRITE):
                writes[x.addr] = x.data
            if(x.type == FileRecord.READ):
                reads[x.addr] = x.data
        prev_k = -1000
        for k in sorted(writes.keys()):
            if 
                    
        
