This is just a simple repo I use to practice the basic skill of modern C++

Notice:
1. How to set launch.json configuration for vs code:
{
    "version": "0.2.0",
    "configurations": [
      {
        "name": "C++ Launch",
        "type": "cppdbg",
        "request": "launch",
        "program": "${workspaceFolder}/build/mordern_cpp/reactor/EchoServer",
        "stopAtEntry": false,
        "cwd": "${workspaceFolder}",
        //"customLaunchSetupCommands": [
        //  { "text": "target-run", "description": "run target", "ignoreFailures": false }
        //],
        "targetArchitecture": "x86_64",
        "launchCompleteCommand": "exec-run",
        "linux": {
          "MIMode": "gdb",
          "miDebuggerPath": "/usr/bin/gdb"
        },
        "osx": {
          "MIMode": "lldb"
        },
        "windows": {
          "MIMode": "gdb",
          "miDebuggerPath": "C:\\MinGw\\bin\\gdb.exe"
        }
      }
    ]
  }

Compile option:

1. cmake -DLIBOSMIUM_DIR=/home/muzi/Documents/exer_repo_cpp/libs/libosmium .., be careful, you have to choose your own libosmium path to replace "/home/muzi/Documents/exer_repo_cpp/libs/libosmium"

2. cmake -DLIBOSMIUM_DIR=/home/muzi/Documents/exer_repo_cpp/libs/libosmium -DLIBMAPNIK_DIR=/home/muzi/Documents/exer_repo_cpp/libs/external/mapnik .., be careful, you have to choose your own libosmium path to replace "/home/muzi/Documents/exer_repo_cpp/libs/external/mapnik/include"

3. Solve the struct conflict by changing the libproj version to 7.2.1 in /usr/local/

Create PostgreSql:
1. 
sudo -u postgres createuser osmuser
sudo -u postgres createdb -E UTF8 -O osmuser osmdb
sudo -u postgres psql -c "CREATE EXTENSION postgis;" -d osmdb
sudo -u postgres psql -c "CREATE EXTENSION hstore;" -d osmdb
Means user as osmuser
and
database as osmdb
have been created

sudo -u postgres psql -c "ALTER USER osmuser WITH PASSWORD 'password';"
Means password as 'password' has been created

osm2pgsql -d osmdb --create --slim -G --hstore \
  --tag-transform-script /path/to/openstreetmap-carto-de/openstreetmap-carto.lua \
  -C 2000 --number-processes 4 \
  test.osm.pbf
Means the data of test osm file has been added into the database

Dont forget to run ./scripts/get-external-data.py under openstreetmap-carto-de (add username: osmuser and password: password in external-data.yml) (if you get error like: FileNotFoundError: [Errno 2] No such file or directory: 'ogr2ogr', run sudo apt update and sudo apt install gdal-bin)

Run psql -U osmuser -d osmdb -f functions.sql

How to run postgresql? sudo systemctl start postgresql

How to import osm to database?
PGPASSWORD=password osm2pgsql -U osmuser --slim -d osmdb -C 3600 --hstore -S /home/muzi/Documents/tile_server/openstreetmap_carto/openstreetmap-carto-de/openstreetmap-carto.style --tag-transform-script /home/muzi/Documents/tile_server/openstreetmap_carto/openstreetmap-carto-de/openstreetmap-carto.lua test.osm.pbf

Install fonts:
sudo apt-get install fonts-noto-cjk fonts-noto-hinted fonts-noto-unhinted fonts-hanazono

Start render server:
1. systemctl start postgresql
2. sudo renderd -f -c /etc/renderd.conf

Start render request
3. render_list -a -m mapnik -z 0 -Z 2
