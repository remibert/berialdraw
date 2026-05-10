To create a TTF file, you need FontForge and to open the SVG file:

## Steps

1. **Right-click** on the black square and **delete** it.
2. **Select all characters** in the font.

3. Go to the menu: `Element > Simplify > Simplify`.
   ![Simplify](./images/simplify.png)

4. Go to the menu: `Element > Round > Round to int`.
   ![Round to int](./images/round-int.png)

5. Go to the menu: `Element > Correct direction`.
   ![Correct direction](./images/fix-direction.png)

6. Go to the menu `Codage/Compact`
   ![Compact](./images/compact.png)

7. Go to the menu: `Element > Transformations`
	 ![General](./images/move.png)
   ![General](./images/move-dialog.png)

8. Go to the menu: `Element > Font Info`:
   ![General](./images/postscript.png)

   - OS/2 General
   ![General](./images/general.png)

   - OS/2 Metric
   ![Metric](./images/metric.png)


 - **Regular font**

   - OS/2 Regular
   ![Miscellaneous](./images/miscellaneous-regular.png)

   - OS/2 Panose
   ![Panose bold](./images/panose-regular.png)

   - For MAC regular
   ![Regular](./images/regular.png)

 - **Bold font**

   - OS/2 Bold
   ![Miscellaneous Bold](./images/miscellaneous-bold.png)

   - OS/2 Panose
   ![Panose bold](./images/panose-bold.png)

   - For MAC bold
   ![Bold](./images/bold.png)

1. Go to `File > Generate Fonts`:
   - Choose the directory and name the file `Cerial.ttf`
   - Click **Yes** when prompted
   - Click **Generate**
