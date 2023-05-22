#!/bin/sh

if [ -z $1 ]
then
    echo "Usage: $0 UI"
    exit 1
fi

if [ -d "cake-sources" ]
then
    mkdir -p tools/Addins/Cake.FileHelpers.6.1.3
    unzip -o cake-sources/cake.filehelpers.6.1.3.nupkg -d tools/Addins/Cake.FileHelpers.6.1.3/
    dotnet tool restore --add-source cake-sources
    dotnet cake --target=Publish --prefix=/app --ui=$1 --self-contained --sources=nuget-sources
else
    dotnet tool restore
    dotnet cake --target=Publish --prefix=/app --ui=$1 --self-contained
fi

dotnet cake --target=Install