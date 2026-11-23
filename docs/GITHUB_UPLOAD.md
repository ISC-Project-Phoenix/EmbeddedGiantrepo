# Updating the official ISC repository safely

The ZIP is a **clean project snapshot**, not a Git repository. It intentionally excludes `.git`, `.pio`, compiler database files, and the earlier accidental nested Git gitlink.

The recommended procedure is to work in your existing Git clone at:
`C:\Users\saadi\EmbeddedGiantrepotest`

1. Back up local changes first.
2. Copy the *contents* of the extracted `EmbeddedGiantrepo` folder into the top level of your existing clone. Keep `firmware/`, `docs/`, `ros2/`, `.gitignore`, etc. at the root.
3. In CMD, check the state:

```cmd
cd /d C:\Users\saadi\EmbeddedGiantrepotest
git remote -v
git status
```

4. If the accidental nested gitlink is still tracked, remove only its index entry:

```cmd
git rm --cached EmbeddedGiantrepotest
```

Only run that command if the gitlink exists. Do **not** delete any separate repo or local folder without checking its contents.

5. If `firmware/compile_commands.json` is still tracked, untrack it:

```cmd
git rm --cached firmware/compile_commands.json
```

The file is ignored by `.gitignore` in this archive.

6. From the `firmware` directory, build and test:

```cmd
cd firmware
C:\Users\saadi\.platformio\penv\Scripts\pio.exe test -e native
C:\Users\saadi\.platformio\penv\Scripts\pio.exe run -e nucleo_f303k8
cd ..
```

7. Review the changes, stage intentional project files, and push **only after tests succeed**:

```cmd
git diff --stat
git add .gitignore .gitattributes README.md FIXES.md docs firmware ros2 scripts test
git status
git commit -m "Organize and update Project Phoenix firmware"
git pull --rebase isc main
git push isc main
```

If `git status` lists unrelated files, inspect them before committing. If any Git step fails or reports conflicts, stop and resolve that error rather than force-pushing.
