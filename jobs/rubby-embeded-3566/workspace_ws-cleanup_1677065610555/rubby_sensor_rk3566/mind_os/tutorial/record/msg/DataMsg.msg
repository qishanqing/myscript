namespace tutorial.record

message InnerVector {
    vector_data: [uint32]
}

message DataMsg {
    stamp: uint64
    data: double
    inner_vector_data: [InnerVector]
}