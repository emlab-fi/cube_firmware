echo "Generating protocols"
nanopb_generator --strip-path -L "#include <nanopb/%s>" -f protocols/cube.options protocols/cube.proto
echo "Generated protocols"
