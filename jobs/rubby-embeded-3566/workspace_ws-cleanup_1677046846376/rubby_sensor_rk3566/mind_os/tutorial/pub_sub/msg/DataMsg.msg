namespace tutorial.pub_sub

message InnerVector {
    vector_data: [uint32]
}

message DataMsg {
    stamp: uint64
    data: double
    vector_data_1: [uint32]
    vector_data_2: [[uint32]]
    vector_data_3: [InnerVector]
}