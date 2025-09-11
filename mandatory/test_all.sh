for file in $(ls maps/good/*)             
do
valgrind ./cub3D $file && (cat | echo -n)
done