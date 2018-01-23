function [dst_counter] = AugmentSketches(src_img,imageSide,dst_cat_dir)
% src_img = Path to 16-bit PNG sketch
% dst_cat_dir = Path to where transformed images must be stored

% Original -> 1
% NOTE : png images are 16-bit. We need to convert them to 8-bit
I = imread(src_img);
I = im2uint8(I);

% IMPORTANT: thicken the sketch (COMMENT OUT the 2 lines below if sketch is already thickened)
se = strel('square',5);
I = uint8(255)-uint8(imdilate(uint8(255)-uint8(I),se,'same'));

dst_counter = 0;
dst_counter = dst_counter + 1;
str = sprintf('%s/%d.png',dst_cat_dir,dst_counter);
I = uint8(I);
imwrite(I,str);
%fprintf('(%d)',dst_counter);

% Mirroring -> 1
dst_counter = dst_counter + 1;
str = sprintf('%s/%d.png',dst_cat_dir,dst_counter);
I_mirrored = fliplr(I);
imwrite(I_mirrored,str);
%fprintf('(%d)',dst_counter);

% Rotate -> 4
rot_arr = [15 5 -5 -15];
pad_width = 70;
I_padded = padarray(I,[pad_width pad_width],255);
for i = 1:length(rot_arr)
    dst_counter = dst_counter + 1;
    str = sprintf('%s/%d.png',dst_cat_dir,dst_counter);     
    I_rot = imrotate(I_padded,rot_arr(i), 'crop');   
    I_new = I_rot(pad_width+1:end-pad_width,pad_width+1:end-pad_width);    
    imwrite(I_new,str);
end
%fprintf('(%d)',dst_counter);

% Shifts -> 16
shift_arr = [10 5 -5 -10];
for m = 1:length(shift_arr)
    for n = 1:length(shift_arr)
        dst_counter = dst_counter + 1;
        str = sprintf('%s/%d.png',dst_cat_dir,dst_counter);     
        I_shifted = circshift(I,[shift_arr(m) shift_arr(n)]);
        imwrite(I_shifted,str);
    end
end

% Central zoom in and out  -> 8 
zoom_factors = [0.07 0.03 -0.03 -0.07];
for i = 1:length(zoom_factors)
	% Scale along width
	scale_pix_width = double(size(I,2))*abs(zoom_factors(i));
	new_height = size(I,1);
	new_width  = round(size(I,2) + sign(zoom_factors(i)) * scale_pix_width);
	I_scaled = imresize(I,[new_height new_width]);
	if( size(I_scaled,2) > size(I,2) ) % expansion
        	dst_counter = dst_counter + 1;
        	str = sprintf('%s/%d.png',dst_cat_dir,dst_counter);     
		p = floor((size(I_scaled,1) - size(I,1))/2);
		I_scaled = I_scaled(p+1:p+size(I,1),p+1:p+size(I,2));
        	imwrite(I_scaled,str);
	end
	if( size(I_scaled,2) < size(I,2) ) % contraction
        	dst_counter = dst_counter + 1;
        	str = sprintf('%s/%d.png',dst_cat_dir,dst_counter);     
		I_shrunk = I;
		I_shrunk(1:size(I_scaled,1),1:size(I_scaled,2)) = I_scaled;
        	imwrite(I_shrunk,str);
	end
	% Scale along height
	scale_pix_height = double(size(I,1))*abs(zoom_factors(i));
	new_width = size(I,2);
	new_height  = round(size(I,1) + sign(zoom_factors(i)) * scale_pix_height);
	I_scaled = imresize(I,[new_height new_width]);
	if( size(I_scaled,1) > size(I,1) ) % expansion
        	dst_counter = dst_counter + 1;
        	str = sprintf('%s/%d.png',dst_cat_dir,dst_counter);     
		p = floor((size(I_scaled,1) - size(I,1))/2);
		I_scaled = I_scaled(p+1:p+size(I,1),1:size(I,2));
        	imwrite(I_scaled,str);
	end
	if( size(I_scaled,1) < size(I,1) ) % contraction
        	dst_counter = dst_counter + 1;
        	str = sprintf('%s/%d.png',dst_cat_dir,dst_counter);     
		I_shrunk = uint8(255*ones(size(I)));
		I_shrunk(1:size(I_scaled,1),1:size(I_scaled,2)) = uint8(I_scaled);
        	imwrite(I_shrunk,str);
	end
end
%fprintf('(%d)',dst_counter);

end