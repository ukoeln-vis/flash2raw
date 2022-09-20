OUTDIR=/scratch/szellma1

for i in {150..999}
do
  #./flash2cells /scratch/dseifrie/SN_explosion/MC1-HD-px25/FLASH_HDF5_FILES/SILCC_hdf5_plt_cnt_0${i} --var temp --o ${OUTDIR}/MC1-HD-px25-temp_256x256x256_t0${i}
  ./cells2raw ${OUTDIR}/MC1-HD-px25-temp_256x256x256_t0${i}.scalars ${OUTDIR}/MC1-HD-px25-temp_256x256x256_t0${i}.cells
  mv temp.raw ${OUTDIR}/MC1-HD-px25-temp_256x256x256_t0${i}.dat
done
