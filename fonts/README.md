To create a TTF file, you need FontForge and to open the SVG file:

## Steps

1. **Right-click** on the black square and **delete** it.
2. **Select all characters** in the font.
3. Go to the menu: `Element > Simplify > Simplify`.
4. Go to the menu: `Element > Round > Round to int`.
5. Go to the menu: `Element > Correct direction`.
6. Go to the menu: `Element > Font Info`:
   - **PostScript Names**:
     - **Font Name**: `Berial` or `Berial-Bold`
     - **Family Name**: `Berial`
     - **Full Name**: `Berial` or `Berial Bold`
     - **Weight**: `Book` or `Bold`
   - **General**
     - **Ascent**: 1024
     - **Cadratin**: 1024
   - **OS/2**:
     - **Misc**:
       - **OS/2 version**: `1`
       - **PFM Family**: `Fixed Pitch`
       - **Style**: `Regular` or `Bold`
     - **Panose**:
       - **Weight**: `Any` or `Bold`
       - **Proportion**: `Monospaced`
7. Go to `File > Generate Fonts`:
   - Choose the directory and name the file `Berial.ttf`
   - Click **Yes** when prompted
   - Click **Generate**
