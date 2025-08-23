cmake_minimum_required(VERSION 3.10)
project(DuckDB_TEST C)

add_executable(DuckDB_TEST main.c)

# Add Include Directory
target_include_directories(DuckDB_TEST PRIVATE
    /home/sfcyberbox/Documents/Software_Libraries/C/DuckDB/x86_64
)

# Add Link Directory
target_link_directories(DuckDB_TEST PRIVATE
    /home/sfcyberbox/Documents/Software_Libraries/C/DuckDB/x86_64
)

# Link against DuckDB
target_link_libraries(DuckDB_TEST PRIVATE duckdb)
