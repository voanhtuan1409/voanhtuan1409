import array, os, sys

os.chdir('..')
workDir = os.getcwd()

inputDir = "%s\input" % workDir
expectDir = "%s\expect" % workDir

#Convert input
listAPIs = os.listdir(inputDir)

print('Start converting input !!!')
#Each API
for i in range (0, len(listAPIs)):
    if 'RDM_' in listAPIs[i]:
        inputFolder = "%s\%s" % (inputDir, listAPIs[i])
        listFiles = os.listdir(inputFolder)

        #Determine data type (one time per API)
        if '_U' in listAPIs[i]:
            dataType = ['H', 'H', 'H']
            
        elif 'RDM_FldAmpDBConv' in listAPIs[i]:
            dataType = ['H']
          
        elif 'RDM_SLL' in listAPIs[i] or 'RDM_SRL' in listAPIs[i]:
            dataType = ['H', 'H']
          
        elif 'RDM_SRA' in listAPIs[i]:
            dataType = ['h', 'H']

        elif 'RDM_Copy' in listAPIs[i]:
            dataType = ['h', 'l', 'h']

        #The rest
        else:
            dataType = ['h', 'h', 'h']

        dataType += 'h'
        #Each csv file
        for j in range (0, len(listFiles)):
            if ".csv" in listFiles[j]:
                fileCSVDir = inputDir + "\\" + listAPIs[i] + "\\" + listFiles[j]
                fileBin = listFiles[j][0:(len(listFiles[j]) - 4)] + '.bin'
                fileBinDir = inputDir + "\\" + listAPIs[i] + "\\" + fileBin

                if os.path.isfile(fileBinDir) == 0:
                    with open(fileCSVDir, 'rt') as f:
                        texts = f.read().split('\n')
                        values = []
                        row = 1
                        with open(fileBinDir, 'wb') as out:
                            for text in texts:
                                entries = text.split(',')
                                entries = filter(None, entries)
                                values = [int(x) for x in entries]

                                #Exception: data of RDM_VerticalMean has multiple rows.
                                if 'RDM_VerticalMean_S' in listAPIs[i]:
                                    bin_vals = array.array('h', values)

                                elif 'RDM_VerticalMean_U' in listAPIs[i]:
                                    bin_vals = array.array('H', values)
                                else:
                                    bin_vals = array.array(dataType[row - 1], values)
                                    
                                bin_vals.tofile(out)
                                row += 1

        print(listAPIs[i] + ': Done')
        print('Running')
print('Finish converting input !!!')


#Convert expected output
listAPIs = os.listdir(expectDir)

print('Start converting expected output !!!')
#Each API
for i in range (0, len(listAPIs)):
    if 'RDM_' in listAPIs[i]:
        inputFolder = "%s\%s" % (expectDir, listAPIs[i])
        listFiles = os.listdir(inputFolder)

        #Determine data type (one time per API)
        if '_U' in listAPIs[i]:
            dataType = ['H']

        elif 'GetVersion' in listAPIs[i]:
            dataType = ['L']
            
        elif 'RDM_SLL' in listAPIs[i] or 'RDM_SRL' in listAPIs[i]:
            dataType = ['H']

        elif 'RDM_Abs' in listAPIs[i]:
            dataType = ['H']

        #The rest
        else:
            dataType = ['h']

        dataType += 'h' #for the last null value
        
        #Each csv file
        for j in range (0, len(listFiles)):
            if ".csv" in listFiles[j]:
                fileCSVDir = expectDir + "\\" + listAPIs[i] + "\\" + listFiles[j]
                fileBin = listFiles[j][0:(len(listFiles[j]) - 4)] + '.bin'
                fileBinDir = expectDir + "\\" + listAPIs[i] + "\\" + fileBin

                if os.path.isfile(fileBinDir) == 0:
                    with open(fileCSVDir, 'rt') as f:
                        texts = f.read().split('\n')
                        values = []
                        row = 1
                        with open(fileBinDir, 'wb') as out:
                            for text in texts:
                                entries = text.split(',')
                                entries = filter(None, entries)
                                values = [int(x) for x in entries]

                                bin_vals = array.array(dataType[row - 1], values)
                                bin_vals.tofile(out)
                                row += 1

        print(listAPIs[i] + ': Done')
        print('Running')
print('Finish converting expected output !!!')
