Convert AMR FLASH to structured raw
===================================

Create two files `.scalars` and `.cells` from FLASH file
```
./flash2cells <flash-file> --var dens --o <out-file-base>
```

Convert to raw (256x256x256, edit cells2raw.cpp:131 for other dimensions, TODO...)
To fix the value range across a series of files, assign these in cells2raw.cpp:189 ff (TODO...)

Create a file called temp.dat in CWD:
```
./cells2raw <out-file-base>.scalars <out-file-base>.cells
```
