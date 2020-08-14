# FastIndexer
Fast find index feature in C++\/Windows.  
You can assign blank index and release by O(1).  


```
FastIndexer indexer(0) // you can assign until 64 indexes.

const u32 index0 = indexer.assign(); //asign index 0.
const u32 index1 = indexer.assign(); //asign index 1.
const u32 index2 = indexer.assign(); //asign index 2.

indexer.release(index1); // release assigned index 1.

const u32 index3 = indexer.assign(); assign index 1.
```

But if you require huge indexes, you need a lot of memory.  
```
FastIndexer indexer0(0); // you can assign until 64 indexes. requires 8 byte.
FastIndexer indexer1(1); // you can assign until 4096 indexes. requires 520 byte.
FastIndexer indexer2(2); // you can assign until 262144 indexes. requires 65544 byte.
FastIndexer indexer3(3); // you can assign until 16777216 indexes. requires 6291464 byte.
...
```

**I have implemented assign()/release() APIs to be thread-safe.  
But these are not tested. So I still cannot guarantee these APIs work correctly.**
