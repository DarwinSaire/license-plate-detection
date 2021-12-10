function DetectLicense

% Segmentação de Placas de Veículo Baseado em Morfologia Matemática
% Darwin Saire Pilco
% UNICAMP

path_ori = 'LicensePlates/orig/';
path_label = 'LicensePlates/label/';
Files_ori = dir(strcat(path_ori ,'*.pgm'));
Files_label = dir(strcat(path_label,'*.pgm'));
counter = 0;
for k=1:length(Files_ori)
   FileNames_ori = Files_ori(k).name;
   FileNames_label = Files_label(k).name;
   counter = counter + placaDetected( strcat(path_ori,FileNames_ori), strcat(path_label,FileNames_label), FileNames_ori);
end
fprintf( 'Total de imagens: %d\n', length(Files_ori) );
fprintf( 'Elementos detetados: %d  aceptacao: %.3f\n', counter, (counter * 100.0 )/length(Files_ori) );

function is_detected = placaDetected( filename_ori, filename_label, filename )

image_label = imread ( filename_label );
image = imread (filename_ori);
image_eq = histeq(image);
%imwrite (image, '1_img_original.jpg');

% Top Hat Black ( Subtracao do Fechamento pelo Original ) 
% Resalta as partes escuras das zonas claras
%
disk = strel ('disk', 3 );
img = imbothat(image_eq,disk);
%imwrite (img, '2_img_BotHat.jpg');

% sobel vertical ( nao foi usado )
h = fspecial('sobel');
h = h';
ImgSobel = imfilter( img,h);
%figure, imshow( ImgSobel );
%imwrite (binImg, 'img_sobel.jpg');

% Binarizacao pelo Metodo Otsu 
binImg = im2bw (ImgSobel, graythresh(ImgSobel));
%binImg = im2bw (img, graythresh(img));
%imwrite (binImg, '3_img_bin.jpg');

% Fechamento linear horizontal 
% Separacao entre os caracteres possiveis. Isto converte o
% conjunto dos caracteres num retangulo
%
% #################################################################
hline = strel ('rectangle',[1,15]);%22
binImgClosed = imclose (binImg, hline);
%imwrite (binImgClosed, '4_img_close.jpg');

% Abertura Linear Vertical - Eliminação Altura Minima
% Eliminamos os objetos de altura inferior à altura minima dos caracteres
%
vlineMin = strel ('rectangle',[10,15]);
binImgOpenedMin = imopen (binImgClosed, vlineMin);
%binImgOpenedMin = imopen (binImgClosed, vlineMin);
%imwrite (binImgOpenedMin, '5_img_abertura_vertical_minima.jpg');

% Abertura Linear Vertical - Eliminacao Altura Maxima
% Abertura para eliminar os objetos de altura inferior a altura maxima 
% dos caracteres
%
vlineMax = strel ('line', 10, 50);
binImgOpenedMax = imopen (binImgOpenedMin, vlineMax);
%imwrite (binImgOpenedMax, '6_img_abertura_vertical_maxima.jpg');

% Alargamento da Placa 
% Aplica-se uma dilatacao para assegurar de que se obtem a placa inteira
%
%binImgOpenedMax = binImgClosed;
dilata = strel('square',25);
plateImg = imdilate(binImgOpenedMax,dilata);
%imwrite (plateImg, '7_img_possivel_placa.jpg');

% Definição de Imagens
%
image = im2uint8 (image);
img = im2uint8 (img);
binImg = im2uint8 (binImg);
binImgClosed = im2uint8 (binImgClosed);
binImgOpenedMax = im2uint8 (binImgOpenedMax);
binImgOpenedMin = im2uint8 (binImgOpenedMin);
plateImg = im2uint8 (plateImg);
imgFinal = imadd (image, plateImg);
imgFinalSo = imsubtract (plateImg, image);
%imwrite (imgFinal, 'Imagem final.jpg');
%imwrite (imgFinalSo, 'Imagem final_.jpg');
%imwrite (plateImg, 'Final_Final.jpg' );
imwrite (imgFinalSo, strcat('result/',filename,'.jpg'));

% Comparar imagem com base
% Compara os componente detectados com imagem label
%
I_comparar = image_label  - ( 255 - plateImg );

% Plot das Etapas
%
%scrsz = get(0,'ScreenSize');
%figure('Position',[1 scrsz(4)/2 scrsz(3)/2 scrsz(4)],'ToolBar','none', 'Name','Etapas da Segmentação de Placas', 'NumberTitle', 'off', 'Renderer','OpenGL')
%subplot(3,3,1), subimage (img), axis off, title ('TopHatBlack')
%subplot(3,3,2), subimage (binImg), axis off, title ('Binarizada')
%subplot(3,3,3), subimage (binImgClosed), axis off, title ('Fechada Linear')
%subplot(3,3,4), subimage (binImgOpenedMin), axis off,title ('Aberta Min.')
%subplot(3,3,5), subimage (binImgOpenedMax), axis off,title ('Abertura Max.')
%subplot(3,3,6), subimage (plateImg), axis off, title ('Placa Componentes')
%subplot(3,3,7), subimage (imgFinal), axis off, title ('Imagem Final')
%subplot(3,3,8), subimage (imgFinalSo), axis off, title ('Placa')
%subplot(3,3,9), subimage (I_comparar), axis off, title ('Comparar')
%imwrite (I_comparar, strcat('result/',filename,'.jpg'));

if max( max( I_comparar ) ) == 255
    is_detected = 1;
else
    is_detected = 0;
end
