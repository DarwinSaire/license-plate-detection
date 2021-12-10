# License Plate Detection
Automatic license plate detection

#### Pipeline to project

1. Selection  of  candidates

   	* Equalization

    * Top-Hat
    * Sobel
    * Binarization
    * Horizontal closing
    * Vertical opening
    * Dilation

2. Feature extractions

   - Local  Binary  Pattern  (LBP)
   - Histogram  of  Oriented  Gradients  (HOG)
   - Scale-Invariant  Feature  Transform  (SIFT)

3. Watershed segmentation

   - Plate detection
   - Plate boxing
   - Seeds for segmentation
   - Plate basins

4. Correction of the plate

   - Camera calibration, parallelogram approximation
   - Principal Component Analysis (PCA)

5. Characters separation 

   - Plate normalization
   - Local plate threshold
   - Plate rotation
   - Horizontal and vertical projection
   - Noise Elimination (clean)
   - Characters count
   - Getting separate characters

   

#### Run the code

Select Candidate 

```c++
gcc -std=c11 iftSelectCandidates.c -o SelectCandidates -L../lib -L../libsvm -lift -llapack -lblas -lm -lsvm -lstdc++ -I../include -I../libsvm
./SelectCandidates ../../LicensePlates/orig ../../LicensePlates/result_label
```

Extract Features

```c++
gcc -std=c11 iftExtractFeatures.c -o ExtractFeatures -L../lib -L../libsvm -lift -llapack -lblas -lm -lsvm -lstdc++ -I../include -I../libsvm
./ExtractFeatures ../../LicensePlates/orig/ ../../LicensePlates/label/ ../../LicensePlates/result_label/ opf_dataset.data file.txt
```

Training SVM 

```c++
gcc -std=c11 iftDesignClassifier.c -o DesignClassifier -L../lib -L../libsvm -lift -llapack -lblas -lm -lsvm -lstdc++ -I../include -I../libsvm
./DesignClassifier opf_dataset.data HOG_classifier
```

Detect Plate

```c++
gcc -std=c11 iftDetectPlate.c -o DetectPlate  -L../lib -L../libsvm -lift -llapack -lblas -lm -lsvm -lstdc++ -I../include -I../libsvm
./DetectPlate ../../LicensePlates/orig/ ../../LicensePlates/label/ HOG_classifier ../../LicensePlates/result_delimiter
```

