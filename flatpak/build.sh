#!/bin/sh

if [ -z $1 ]
then
    echo "Usage: $0 UI"
    exit 1
fi

if [ -d "nuget-sources" ]
then
    mv flatpak/NuGet.config ./
fi

dotnet tool restore
dotnet cake --target=Publish --prefix=/app --ui=$1 --self-contained

dotnet cake --target=Install
