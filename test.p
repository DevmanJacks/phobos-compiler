var a: Int32 = 5

interface Stream {
    ReadBytes(bytes: bytesToRead: Int) -> []Byte
    Write(bytes: []Byte, bytesToWrite: Int)
}

func ReadLine(reader: StreamReader) {

}

func ReadBytes(reader: BinaryReader, bytesToRead: Int) -> []Byte {

}

func (p: *Parser) ParseDecl() -> Decl {
    switch currToken.Kind {
        case Token.Var
    }
}

func (p: *Parser) Parse() {
    while currToken.Kind != Token.EOF {
        decl := p.ParseDecl()
    }
}