Imports System
Imports System.IO
Imports System.Text
Public Class Form1
    Dim bmp As Bitmap

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        'opens filestream
        Dim fs As System.IO.FileStream

        'If there exists a Program 1.bmp in the directory
        '(For new users, there will not be "Program 1-6.bmp)
        If My.Computer.FileSystem.FileExists("Program1.bmp") Then
            fs = New System.IO.FileStream("Program1.bmp", IO.FileMode.Open, IO.FileAccess.Read)

            programOneImage.Image = System.Drawing.Image.FromStream(fs)
            'for formatting purposes, we do a RotateFlip so the images are
            'able to be displayed correctly
            programOneImage.Image.RotateFlip(7)
            fs.Close()
        End If

        If My.Computer.FileSystem.FileExists("Program2.bmp") Then
            fs = New System.IO.FileStream("Program2.bmp", IO.FileMode.Open, IO.FileAccess.Read)

            programTwoImage.Image = System.Drawing.Image.FromStream(fs)
            programTwoImage.Image.RotateFlip(7)
            fs.Close()
        End If


        If My.Computer.FileSystem.FileExists("Program3.bmp") Then
            fs = New System.IO.FileStream("Program3.bmp", IO.FileMode.Open, IO.FileAccess.Read)

            programThreeImage.Image = System.Drawing.Image.FromStream(fs)
            programThreeImage.Image.RotateFlip(7)
            fs.Close()
        End If


        If My.Computer.FileSystem.FileExists("Program4.bmp") Then
            fs = New System.IO.FileStream("Program4.bmp", IO.FileMode.Open, IO.FileAccess.Read)

            programFourImage.Image = System.Drawing.Image.FromStream(fs)
            programFourImage.Image.RotateFlip(7)
            fs.Close()
        End If

        If My.Computer.FileSystem.FileExists("Program5.bmp") Then
            fs = New System.IO.FileStream("Program5.bmp", IO.FileMode.Open, IO.FileAccess.Read)

            programFiveImage.Image = System.Drawing.Image.FromStream(fs)
            programFiveImage.Image.RotateFlip(7)
            fs.Close()
        End If


        If My.Computer.FileSystem.FileExists("Program6.bmp") Then
            fs = New System.IO.FileStream("Program6.bmp", IO.FileMode.Open, IO.FileAccess.Read)

            programSixImage.Image = System.Drawing.Image.FromStream(fs)
            programSixImage.Image.RotateFlip(7)

            fs.Close()
        End If

        If My.Computer.FileSystem.FileExists("Back.bmp") Then
            fs = New System.IO.FileStream("Back.bmp", IO.FileMode.Open, IO.FileAccess.Read)

            backGroundImage.Image = System.Drawing.Image.FromStream(fs)
            backGroundImage.Image.RotateFlip(7)

            fs.Close()
        End If

        If My.Computer.FileSystem.FileExists("names.csv") Then
            Using MyReader As New Microsoft.VisualBasic.FileIO.TextFieldParser("names.csv")

                'Allows the program to read a file from the directory with a specified delimiter
                MyReader.TextFieldType = Microsoft.VisualBasic.FileIO.FieldType.Delimited
                'the delimiter for a new line is a comma
                MyReader.Delimiters = New String() {","}
                Dim currentRow As String()
                Dim rowNum As Integer = 1
                'Loop through all of the fields in the file. 
                'If any lines are corrupt, report an error and continue parsing. 
                While Not MyReader.EndOfData
                    Try
                        currentRow = MyReader.ReadFields()
                        ' Include code here to handle the row.
                        If rowNum = 1 Then
                            'Do nothing
                        ElseIf rowNum = 2 Then
                            'Textbox reads the program's name
                            programTwoTextbox.Lines = currentRow
                        ElseIf rowNum = 3 Then
                            programThreeTextbox.Lines = currentRow
                        ElseIf rowNum = 4 Then
                            programFourTextbox.Lines = currentRow
                        ElseIf rowNum = 5 Then
                            programFiveTextbox.Lines = currentRow
                        Else
                            programSixTextbox.Lines = currentRow
                        End If
                        'increment for the next row
                        rowNum += 1
                    Catch ex As Microsoft.VisualBasic.FileIO.MalformedLineException
                        MsgBox("Line " & ex.Message &
                        " is invalid.  Skipping")
                        rowNum += 1
                    End Try
                End While
            End Using
        End If

    End Sub
    Private Sub addProgramOneButton_Click(sender As Object, e As EventArgs) Handles addProgramOneButton.Click

        'If button is selected, then the file dialog is opened
        'once image is selected, the picturebox contains the new image
        If OpenFileDialog1.ShowDialog = DialogResult.OK Then
            programOneLabel.Text = OpenFileDialog1.FileName
            programOneImage.ImageLocation = programOneLabel.Text
        End If

    End Sub

    Private Sub addProgramTwoButton_Click(sender As Object, e As EventArgs) Handles addProgramTwoButton.Click

        If OpenFileDialog2.ShowDialog = DialogResult.OK Then
            programTwoLabel.Text = OpenFileDialog2.FileName
            programTwoImage.ImageLocation = programTwoLabel.Text
        End If

    End Sub

    Private Sub addProgramThreeButton_Click(sender As Object, e As EventArgs) Handles addProgramThreeButton.Click

        If OpenFileDialog3.ShowDialog = DialogResult.OK Then
            programThreeLabel.Text = OpenFileDialog3.FileName
            programThreeImage.ImageLocation = programThreeLabel.Text
        End If

    End Sub

    Private Sub addProgramFourButton_Click(sender As Object, e As EventArgs) Handles addProgramFourButton.Click

        If OpenFileDialog4.ShowDialog = DialogResult.OK Then
            programFourLabel.Text = OpenFileDialog4.FileName
            programFourImage.ImageLocation = programFourLabel.Text
        End If

    End Sub

    Private Sub addProgramFiveButton_Click(sender As Object, e As EventArgs) Handles addProgramFiveButton.Click

        If OpenFileDialog5.ShowDialog = DialogResult.OK Then
            programFiveLabel.Text = OpenFileDialog5.FileName
            programFiveImage.ImageLocation = programFiveLabel.Text
        End If

    End Sub

    Private Sub addProgramSixButton_Click(sender As Object, e As EventArgs) Handles addProgramSixButton.Click

        If OpenFileDialog6.ShowDialog = DialogResult.OK Then
            programSixLabel.Text = OpenFileDialog6.FileName
            programSixImage.ImageLocation = programSixLabel.Text
        End If

    End Sub

    Private Sub sendToDeviceButton_Click(sender As Object, e As EventArgs) Handles sendToDeviceButton.Click

        'creates an array for each textbox's contents
        Dim array1() As String = programOneTextbox.Lines
        Dim array2() As String = programTwoTextbox.Lines
        Dim array3() As String = programThreeTextbox.Lines
        Dim array4() As String = programFourTextbox.Lines
        Dim array5() As String = programFiveTextbox.Lines
        Dim array6() As String = programSixTextbox.Lines
        Dim i As Integer = 0

        For Each killProcess As Process In Process.GetProcessesByName("Microcontroller")
            killProcess.Kill()
            killProcess.Close()
        Next

        'if there is already a names.csv, then we delete it so we can have an updated names.csv
        If My.Computer.FileSystem.FileExists("names.csv") Then
            My.Computer.FileSystem.DeleteFile("names.csv")
        End If

        'Starts Titus' ExtractMasterLevel executeable which grabs volume levels and 
        Process.Start("ExtractMasterLevel.exe")

        'checks to see if the picturebox contains an image
        'if the picturebox contains an image, then it creates a new bitmap
        If programOneImage.Image IsNot Nothing Then

            Dim programOneBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programOneGraph As Graphics = Graphics.FromImage(programOneBitmap)

            programOneGraph.DrawImage(programOneImage.Image, programOneBitmap.GetBounds(GraphicsUnit.Pixel))

            'RotateFlip for correct formatting to display on the microcontroller correctly
            programOneBitmap.RotateFlip(7)

            programOneBitmap.Save("Program1.bmp", Imaging.ImageFormat.Bmp)
            'If the picturebox does NOT contain an image then it saves the default option
            '(A black image, which does not get printed on the touchscreen)
        Else
            Dim programOneBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programOneGraph As Graphics = Graphics.FromImage(programOneBitmap)

            programOneBitmap.Save("Program1.bmp", Imaging.ImageFormat.Bmp)

        End If

        If programTwoImage.Image IsNot Nothing Then
            Dim programTwoBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programTwoGraph As Graphics = Graphics.FromImage(programTwoBitmap)

            programTwoGraph.DrawImage(programTwoImage.Image, programTwoBitmap.GetBounds(GraphicsUnit.Pixel))

            programTwoBitmap.RotateFlip(7)

            programTwoBitmap.Save("Program2.bmp", Imaging.ImageFormat.Bmp)
        Else
            Dim programTwoBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programTwoGraph As Graphics = Graphics.FromImage(programTwoBitmap)

            programTwoBitmap.Save("Program2.bmp", Imaging.ImageFormat.Bmp)
        End If

        If programThreeImage.Image IsNot Nothing Then
            Dim programThreeBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programThreeGraph As Graphics = Graphics.FromImage(programThreeBitmap)

            programThreeGraph.DrawImage(programThreeImage.Image, programThreeBitmap.GetBounds(GraphicsUnit.Pixel))

            programThreeBitmap.RotateFlip(7)

            programThreeBitmap.Save("Program3.bmp", Imaging.ImageFormat.Bmp)
        Else
            Dim programThreeBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programThreeGraph As Graphics = Graphics.FromImage(programThreeBitmap)

            programThreeBitmap.Save("Program3.bmp", Imaging.ImageFormat.Bmp)
        End If

        If programFourImage.Image IsNot Nothing Then
            Dim programFourBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programFourGraph As Graphics = Graphics.FromImage(programFourBitmap)

            programFourGraph.DrawImage(programFourImage.Image, programFourBitmap.GetBounds(GraphicsUnit.Pixel))

            programFourBitmap.RotateFlip(7)

            programFourBitmap.Save("Program4.bmp", Imaging.ImageFormat.Bmp)
        Else
            Dim programFourBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programFourGraph As Graphics = Graphics.FromImage(programFourBitmap)

            programFourBitmap.Save("Program4.bmp", Imaging.ImageFormat.Bmp)

        End If

        If programFiveImage.Image IsNot Nothing Then
            Dim programFiveBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programFiveGraph As Graphics = Graphics.FromImage(programFiveBitmap)

            programFiveGraph.DrawImage(programFiveImage.Image, programFiveBitmap.GetBounds(GraphicsUnit.Pixel))

            programFiveBitmap.RotateFlip(7)

            programFiveBitmap.Save("Program5.bmp", Imaging.ImageFormat.Bmp)
        Else
            Dim programFiveBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programFiveGraph As Graphics = Graphics.FromImage(programFiveBitmap)

            programFiveBitmap.Save("Program5.bmp", Imaging.ImageFormat.Bmp)
        End If

        If programSixImage.Image IsNot Nothing Then
            Dim programSixBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programSixGraph As Graphics = Graphics.FromImage(programSixBitmap)

            programSixGraph.DrawImage(programSixImage.Image, programSixBitmap.GetBounds(GraphicsUnit.Pixel))

            programSixBitmap.RotateFlip(7)

            programSixBitmap.Save("Program6.bmp", Imaging.ImageFormat.Bmp)
        Else
            Dim programSixBitmap As New Bitmap(100, 100, Imaging.PixelFormat.Format24bppRgb)

            Dim programSixGraph As Graphics = Graphics.FromImage(programSixBitmap)

            programSixBitmap.Save("Program6.bmp", Imaging.ImageFormat.Bmp)
        End If

        If backGroundImage.Image IsNot Nothing Then
            Dim backgroundBitmap As New Bitmap(480, 320, Imaging.PixelFormat.Format24bppRgb)

            Dim backgroundGraph As Graphics = Graphics.FromImage(backgroundBitmap)

            backgroundGraph.DrawImage(backGroundImage.Image, backgroundBitmap.GetBounds(GraphicsUnit.Pixel))

            backgroundBitmap.RotateFlip(7)

            backgroundBitmap.Save("Back.bmp", Imaging.ImageFormat.Bmp)
        Else
            Dim backgroundBitmap As New Bitmap(480, 320, Imaging.PixelFormat.Format24bppRgb)

            Dim backgroundGraph As Graphics = Graphics.FromImage(backgroundBitmap)

            backgroundBitmap.Save("Back.bmp", Imaging.ImageFormat.Bmp)
        End If



        For i = 0 To array1.GetUpperBound(0)
            File.WriteAllText("names.csv", array1(i) & "," & Environment.NewLine)
        Next i

        For i = 0 To array2.GetUpperBound(0)
            File.AppendAllText("names.csv", array2(i))
        Next i
        File.AppendAllText("names.csv", "" & "," & Environment.NewLine)

        For i = 0 To array3.GetUpperBound(0)
            File.AppendAllText("names.csv", array3(i))
        Next i
        File.AppendAllText("names.csv", "" & "," & Environment.NewLine)

        For i = 0 To array4.GetUpperBound(0)
            File.AppendAllText("names.csv", array4(i))
        Next i
        File.AppendAllText("names.csv", "" & "," & Environment.NewLine)

        For i = 0 To array5.GetUpperBound(0)
            File.AppendAllText("names.csv", array5(i))
        Next i
        File.AppendAllText("names.csv", "" & "," & Environment.NewLine)

        For i = 0 To array6.GetUpperBound(0)
            File.AppendAllText("names.csv", array6(i))
        Next i
        File.AppendAllText("names.csv", "" & "," & Environment.NewLine)

        Process.Start("Microcontroller.exe")

    End Sub

    Private Sub exitButton_Click(sender As Object, e As EventArgs) Handles exitButton.Click

        'Closes all instances of "Microcontroller.exe" and closes the application
        For Each killProcess As Process In Process.GetProcessesByName("Microcontroller")
            killProcess.Kill()
            killProcess.Close()
        Next
        Me.Close()

    End Sub

    Private Sub backButton_Click(sender As Object, e As EventArgs) Handles backButton.Click

        'Allows the user to change the background on the touchscreen.
        'Works the same way as the pictureboxes for each program
        If OpenFileDialog7.ShowDialog = DialogResult.OK Then
            backLabel.Text = OpenFileDialog7.FileName
            backGroundImage.ImageLocation = backLabel.Text

        End If

    End Sub
End Class