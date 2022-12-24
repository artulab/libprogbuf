#!/bin/sh

git log --pretty="- (%cs) %s" > ChangeLog
