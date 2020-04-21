close all;
%% Prepare calibration images
numImages = 8;
files = cell(1, numImages);

for i = 1:numImages
    files{i} = fullfile('checkerboardTest',...
        sprintf('My Snapshot_%d.jpg', i));
end

% Display one of the calibration images
magnification = 75;
I = imread(files{1});
figure; 
imshow(I, 'InitialMagnification', magnification);
title('One of the Calibration Images');

%% Estimate Camera Parameters
% Detect the checkerboard corners in the images.
[imagePoints, boardSize] = detectCheckerboardPoints(files);

% Generate the world coordinates of the checkerboard corners in the
% pattern-centric coordinate system, with the upper-left corner at (0,0).
squareSize = 24; % in millimeters
worldPoints = generateCheckerboardPoints(boardSize, squareSize);

% Calibrate the camera.
imageSize = [size(I, 1), size(I, 2)];
cameraParams = estimateCameraParameters(imagePoints, worldPoints, ...
                                     'ImageSize', imageSize);

% Evaluate calibration accuracy.
figure; 
showReprojectionErrors(cameraParams);
title('Reprojection Errors');


%% Read the Image of Objects to Be Measured
imOrig = imread(fullfile('checkerboardTest','My Snapshot_6.jpg'));
figure; 
imshow(imOrig, 'InitialMagnification', magnification);
title('Input Image');


%% Undistort the image
% Since the lens introduced little distortion, use 'full' output view to illustrate that
% the image was undistored. If we used the default 'same' option, it would be difficult
% to notice any difference when compared to the original image. Notice the small black borders.
[im, newOrigin] = undistortImage(imOrig, cameraParams, 'OutputView', 'full');
figure; 
imshow(im, 'InitialMagnification', magnification);
title('Undistorted Image');


%% Segment Coins
% Convert the image to the HSV color space.
imHSV = rgb2hsv(im);

% Get the saturation channel.
saturation = imHSV(:, :, 2);

% Threshold the image
%    0.0392    0.1647    0.3765

t = graythresh(saturation);
imCoin = (saturation > t);

figure; 
imshow(imCoin, 'InitialMagnification', magnification);
title('Segmented Coins');


%% 
% Convert the image to the HSV color space.
imHSV = rgb2hsv(im);

% Get the saturation channel.
saturation = imHSV(:, :, 2);
% Calculate two threshold value for image
thresh = multithresh(saturation,2);
% Segment the image into three levels using imquantize .
seg_I = imquantize(saturation,thresh);
% Convert segmented image into colour image and display
RGB = label2rgb(seg_I);
figure;
imshow(RGB);




%% Detect coins
% Find connected components.
blobAnalysis = vision.BlobAnalysis('AreaOutputPort', true,...
    'CentroidOutputPort', false,...
    'BoundingBoxOutputPort', true,...
    'MinimumBlobArea', 200, 'ExcludeBorderBlobs', true);
[areas, boxes] = step(blobAnalysis, imCoin);

% Sort connected components in descending order by area
[~, idx] = sort(areas, 'Descend');

% Get the two largest components.
boxes = double(boxes(idx(1:2), :));

% Reduce the size of the image for display.
scale = magnification / 100;
imDetectedCoins = imresize(im, scale);

% Insert labels for the coins.
imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'rectangle', ...
    scale * boxes, 'coin');
figure; imshow(imDetectedCoins);
title('Detected Coins');





