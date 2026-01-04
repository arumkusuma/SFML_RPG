cmake_minimum_required(VERSION 3.10)

# Nama project (Bebas)
project(GameKelompok)

# Gunakan C++ Standard 17 (agar support fitur modern)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Cari Library SFML di Linux
find_package(SFML 2.5 COMPONENTS graphics window system audio network REQUIRED)

# 2. Masukkan SEMUA file .cpp dan .h secara otomatis
# Perintah ini akan mencari semua file berakhiran .cpp dan .h di folder ini
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

# 3. Buat Executable
add_executable(GameKelompok ${SOURCES} ${HEADERS})

# 4. Link Library SFML ke Executable
target_link_libraries(GameKelompok sfml-graphics sfml-window sfml-system sfml-audio sfml-network)

# --- Tambahan Penting untuk Linux ---
# Include directory saat ini agar header file bisa saling memanggil
target_include_directories(GameKelompok PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})