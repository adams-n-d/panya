function coords = track_leds(video_filename, varargin)
%function coords = track_leds(video_filename, varargin)
%coords = number_of_frames by 9 matrix:
%frame# centroidx centroidy redx redy greenx greeny bluex bluey
% INPUT PARAMETERS AND DEFAULT VALUES:
% track_red = 1;%currently assumed this is true
% track_green = 1;%currently assumed this is true
% track_blue = 0;%currently assumed this is false (blue ignored)
% show_video = 1;%play the movie after each chunk processed?
% frames_per_chunk = 100;
% startframe = 1;
% stopframe = [END];
% rthreshval = 254;%red brightness threshold
% gthreshval = 254;%green brightness threshold
% bthreshval = 256;%blue brightness threshold
% maxpixmoved = 5;%throw out coords if rat appeared to move more than this
% per frame
% sd_above_bg = 2;%number, in stand devs, for thresholding for a pixel
% playback_rate = 30;%frame rate when playing movie
% interpolate_each_chunk = 0;%fill in missing coords for each chunk
% interpolate_at_end = 1;%fill in missing coords all at once
% show_plot = 1;%show a plot of the tracked path
% enforce_proximity = 1; %throw out frame when LEDs not near
% maxpixrange = 10; %throw out coord when spread of pixels too high
% values for masking out outside of video frame
% miny = 20;%how far from top
% maxy = 220;%how far down
% minx =  35;%how far from bottom
% maxx = 275;%how far to right
% e.g.,coords = track_leds('my_vid.avi','sd_above_bg', 2,'show_video',0);
%JRM 10/4/10

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%               SET DEFAULTS FOR PARAMETERS            %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
track_red = 1;%currently assumed this is true
track_green = 1;%currently assumed this is true
track_blue = 0;%currently assumed this is false
show_video = 1;
frames_per_chunk = 100;
startframe = [];
stopframe = [];
rthreshval = 253;%red brightness threshold
gthreshval = 253;%green brightness threshold
bthreshval = 256;%blue brightness threshold
maxpixmoved = 5;
sd_above_bg = 1.3;
playback_rate = 30;
interpolate_each_chunk = 0;
interpolate_at_end = 1;
show_plot = 1;
enforce_proximity = 1;
maxpixrange = 8;
miny = 20;
maxy = 220;
minx =  35;
maxx = 275;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%               PARSE INPUT PARAMETERS                 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
noptargin = size(varargin,2);

j=0;
for k= 1:2:noptargin
    if (ischar(varargin{k}))
        j = j +1;
        %fprintf('     %s\n', varargin{k})
        flags{j} = varargin{k}; %#ok<AGROW>
    else
        fprintf('1st in pair of optional arguments must be a string\n');
        
    end
end

v=0;
for k= 2:2:noptargin
    if (isscalar(varargin{k}))
        v = v + 1;
        %fprintf('     %d\n', varargin{k})
        vars(v) = varargin{k}; %#ok<AGROW>
    else
        fprintf('2nd in pair of optional arguments must be number\n');
        
    end
end
if (j ~= v)
    fprintf('must have equal number of optional flags and values\n');
end

%%%LOOK FOR OPTIONAL INPUT PARAMETERS %%%
if(strmatch('track_red', flags, 'exact'))
    track_red = vars(strmatch('track_red', flags, 'exact'));
end
if(strmatch('track_green', flags, 'exact'))
    track_green = vars(strmatch('track_green', flags, 'exact'));
end
if(strmatch('track_blue', flags, 'exact'))
    track_blue = vars(strmatch('track_blue', flags, 'exact'));
    fprintf('Warning: program currently will not track blue LEDs. Flag ignored.\n');
end
if(strmatch('show_video', flags, 'exact'))
    show_video = vars(strmatch('show_video', flags, 'exact'));
end
if(strmatch('frames_per_chunk', flags, 'exact'))
    frames_per_chunk = vars(strmatch('frames_per_chunk', flags, 'exact'));
end
if(strmatch('startframe', flags, 'exact'))
    startframe = vars(strmatch('startframe', flags, 'exact'));
end
if(strmatch('stopframe', flags, 'exact'))
    stopframe = vars(strmatch('stopframe', flags, 'exact'));
end
if(strmatch('rthreshval', flags, 'exact'))
    threshval = vars(strmatch('rthreshval', flags, 'exact'));
end
if(strmatch('gthreshval', flags, 'exact'))
    threshval = vars(strmatch('gthreshval', flags, 'exact'));
end
if(strmatch('bthreshval', flags, 'exact'))
    threshval = vars(strmatch('bthreshval', flags, 'exact'));
end
if(strmatch('maxpixmoved', flags, 'exact'))
    maxpixmoved = vars(strmatch('maxpixmoved', flags, 'exact'));
end
if(strmatch('sd_above_bg', flags, 'exact'))
    sd_above_bg = vars(strmatch('sd_above_bg', flags, 'exact'));
end
if(strmatch('playback_rate', flags, 'exact'))
    sd_above_bg = vars(strmatch('playback_rate', flags, 'exact'));
end
if(strmatch('interpolate_each_chunk', flags, 'exact'))
    sd_above_bg = vars(strmatch('interpolate_each_chunk', flags, 'exact'));
end
if(strmatch('interpolate_at_end', flags, 'exact'))
    sd_above_bg = vars(strmatch('interpolate_at_end', flags, 'exact'));
end
if(strmatch('show_plot', flags, 'exact'))
    sd_above_bg = vars(strmatch('show_plot', flags, 'exact'));
end
if(strmatch('enforce_proximity', flags, 'exact'))
    sd_above_bg = vars(strmatch('enforce_proximity', flags, 'exact'));
end
if(strmatch('maxpixrange', flags, 'exact'))
    sd_above_bg = vars(strmatch('maxpixrange', flags, 'exact'));
end
if(strmatch('minx', flags, 'exact'))
    sd_above_bg = vars(strmatch('minx', flags, 'exact'));
end
if(strmatch('miny', flags, 'exact'))
    sd_above_bg = vars(strmatch('miny', flags, 'exact'));
end
if(strmatch('maxx', flags, 'exact'))
    sd_above_bg = vars(strmatch('maxx', flags, 'exact'));
end
if(strmatch('maxy', flags, 'exact'))
    sd_above_bg = vars(strmatch('maxy', flags, 'exact'));
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%               READ IN VIDEO OBJECT                   %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%create image stucture element of approximately right size (bigger than LEDS)
se = strel('square', 8);

%access file
vidobj = mmreader(video_filename);

if(vidobj.BitsPerPixel~=24)
    fprintf('video color format must be RGB24\n');
end

tot_frames = vidobj.NumberOfFrames;
if(isempty(stopframe))
    stopframe = tot_frames;
end
if(isempty(startframe))
    startframe = 1;
end


tot_num_frames = stopframe-startframe+1;

%keep in mind that the last chunk may not contain
%the full number in frames_per_chunk
numchunks = ceil(tot_num_frames/frames_per_chunk);

%allocate memory for coords
%frame# centroidx centroidy redx redy greenx greeny bluex bluey
coords = zeros(tot_num_frames,9);


absf = 0;
if(show_video)
    mymoviefig = figure;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%          LOOP THOUGH CHUNKS OF VIDEO                 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for chnknum = 1:numchunks
    %figure out startframe and stopframe for this chunk
    chnk_startframe = startframe + (chnknum-1)*frames_per_chunk;
    
    chnk_stopframe = chnk_startframe + frames_per_chunk - 1;
    %last chunk may not contain the full number in frames_per_chunk
    if(chnk_stopframe > stopframe)
        chnk_stopframe = stopframe;
    end
    
    fprintf('Chunk #: %d of %d (frames %d to %d)\n', chnknum, numchunks, chnk_startframe, chnk_stopframe);
    %read one chunk of video
    vidchunk = read(vidobj, [chnk_startframe chnk_stopframe]);
    nframes = size(vidchunk, 4);
    

    
    %allocate memory
    objmask = zeros(vidobj.Height, vidobj.Width, 3, nframes, 'uint8');
    bgmask = ones(vidobj.Height, vidobj.Width, 3, nframes, 'uint8');
    %mask out bright objects that are too big to be LEDs
    for f = 1:nframes
        %these will be logicals--need to recast as uint8's later for masking
        objmask(:,:,1,f) =  (imopen(vidchunk(:,:,1,f),se)) < 25;%mask out bigger red objects: 25 = arbitrary threshold that works
        objmask(:,:,2,f) = (imopen(vidchunk(:,:,2,f),se)) < 25;%mask out bigger green objects
        objmask(:,:,3,f) =  (imopen(vidchunk(:,:,3,f),se)) < 25;%mask out bigger blue objects
        
    end
    
    objmask = repmat(min(objmask,[],3),[1 1 3 1]);%reject if object in any color
    objmask(:,:,3,:) = zeros(vidobj.Height,vidobj.Width,1,nframes, 'uint8');%get rid of blue altogether
    
    %get rid of any pixles that are not bright enough to be LEDs
    tmp1 = vidchunk(:,:,1,:) >= rthreshval;
    tmp2 = vidchunk(:,:,2,:) >= gthreshval;
    tmp3 = vidchunk(:,:,3,:) >= bthreshval;
    threshmask = cat(3,tmp1,tmp2,tmp3);
    clear global tmp1 tmp2 tmp3
    threshchunk = vidchunk .* uint8(threshmask);
    clear global threshmask
    %use unsmoothed images to find mean and stdev: works better than smoothed
    chunkmean = repmat(uint8(mean(vidchunk, 4)),[1 1 1 nframes]);
    chunkstdev = repmat(uint8(std(single(vidchunk),0, 4)), [1 1 1 nframes]);
    
    %mask in which 1 = > [sd_above_bg] # stdevs above mean
    diffmask = uint8(  (int8(vidchunk) - int8(chunkmean))   >  chunkstdev * sd_above_bg);
    
    %make edges of bgmask 0 
    %(size of edges defined by miny, minx, maxy, maxx)
    bgmask(1:miny,:,:,:) = 0;
    bgmask(:,1:minx,:,:) = 0;
    bgmask(maxy:vidobj.Height,:,:,:) = 0;
    bgmask(:,maxx:vidobj.Width,:,:) = 0;
    
    %apply masks to thresholded frames
    modchunk = threshchunk  .* uint8(objmask) .* diffmask .* bgmask ;
    
    [ry rx rz] = ind2sub([vidobj.Height vidobj.Width 1 nframes], find(modchunk(:,:,1,:) >= rthreshval & modchunk(:,:,2,:) < gthreshval ) );
    [gy gx gz] = ind2sub([vidobj.Height vidobj.Width 1 nframes], find(modchunk(:,:,2,:) >= gthreshval & modchunk(:,:,1,:) < rthreshval ) );
    rcoords = zeros(nframes, 3);
    gcoords = zeros(nframes, 3);
    lastr = 1;
    lastg = 1;
    
    for f = 1:nframes
        %f = frame number for just this chunk
        if (range(rx(rz == f))<maxpixrange & range(ry(rz == f))<maxpixrange) %#ok<AND2>
            rcoords(f, :) = [f median(ry(rz == f)) median(rx(rz == f))];
        else
            rcoords(f, :) = [f NaN NaN];
            %"unmark" these detections
            modchunk(ry(rz == f),rx(rz == f),:,f) = vidchunk(ry(rz == f),rx(rz == f),:,f);
        end
        if (range(gx(gz == f))<maxpixrange & range(gy(gz == f))<maxpixrange) %#ok<AND2>
            gcoords(f, :) = [f median(gy(gz == f)) median(gx(gz == f))];
        else
            gcoords(f, :) = [f NaN NaN];
            %"unmark" these detections
            modchunk(gy(gz == f),gx(gz == f),:,f) = vidchunk(gy(gz == f),gx(gz == f),:,f);
        end
        
        if (f > 1)
            %get rid of spurious points
            if(abs(rcoords(f,2)-rcoords(lastr,2))> (f-lastr)*maxpixmoved)
                rcoords(f,2) = NaN;
                rcoords(f,3) = NaN;
            end
            if(abs(rcoords(f,3)-rcoords(lastr,3))> (f-lastr)*maxpixmoved)
                rcoords(f,2) = NaN;
                rcoords(f,3) = NaN;
            end
            if(max(isnan(rcoords(f,:)))==0)%found red
                lastr = f;
            end
            if(abs(gcoords(f,2)-gcoords(lastg,2))> (f-lastg)* maxpixmoved)
                gcoords(f,2) = NaN;
                gcoords(f,3) = NaN;
            end
            if(abs(gcoords(f,3)-gcoords(lastg,3))> (f-lastg)* maxpixmoved)
                gcoords(f,2) = NaN;
                gcoords(f,3) = NaN;
            end
            if(max(isnan(gcoords(f,:)))==0)%found green
                lastg = f;
            end
        end
        
    end
    
    if(enforce_proximity)
        rcoords( (abs(rcoords(:,2)-gcoords(:,2))>10 | abs(rcoords(:,3)-gcoords(:,3))>10), 2:3) = NaN;
        gcoords( (abs(rcoords(:,2)-gcoords(:,2))>10 | abs(rcoords(:,3)-gcoords(:,3))>10), 2:3) = NaN;
    end
    if(interpolate_each_chunk)
        %interpolate missing coords
        rbadind = find(isnan(rcoords(:,2)) | isnan(rcoords(:,3)));
        rgoodind = find(~isnan(rcoords(:,2)) & ~isnan(rcoords(:,3)));
        gbadind = find(isnan(gcoords(:,2)) | isnan(gcoords(:,3)));
        ggoodind = find(~isnan(gcoords(:,2)) & ~isnan(gcoords(:,3)));
        
        rcoords(rbadind, 2:3) = ...      %missing red coords
            round(...                   %rounded to ints
            interp1(...                 %interpolated
            rcoords(rgoodind,1),...      %using known frames
            rcoords(rgoodind, 2:3),...   %and corresponding known coords
            rcoords(rbadind, 1),...      %to find the unknown frames
            'linear',...                %use linear interpolation
            'extrap'...                 %use linear extrapolation for out of range values
            ));
        
        gcoords(gbadind, 2:3) = ...      %missing green coords
            round(...                   %rounded to ints
            interp1(...                 %interpolated
            gcoords(ggoodind,1),...      %using known frames
            gcoords(ggoodind, 2:3),...   %and corresponding known coords
            gcoords(gbadind, 1),...      %to find the unknown frames
            'linear',...                %use linear interpolation
            'extrap'...                 %use linear extrapolation for out of range values
            ));
        
        %get rid of spurious, i.e., negative, interpolations
        rcoords(rcoords<0) = NaN;
        gcoords(gcoords<0) = NaN;
        
    end%end of interpolate_each_chunk
    
  
    
    %mark on the actual video where we think the LEDs are
    for f = 1:nframes
        
        %red marked with green cross
        if(max(isnan(rcoords(f,:)))==0)
            try
                vidchunk([round(rcoords(f,2)) round(rcoords(f,2))+1 round(rcoords(f,2))-1 round(rcoords(f,2))   round(rcoords(f,2))  ], ...
                    [round(rcoords(f,3)) round(rcoords(f,3))   round(rcoords(f,3))   round(rcoords(f,3))+1 round(rcoords(f,3))-1], ...
                    1 , f) = 50;
                vidchunk([round(rcoords(f,2)) round(rcoords(f,2))+1 round(rcoords(f,2))-1 round(rcoords(f,2))   round(rcoords(f,2))  ], ...
                    [round(rcoords(f,3)) round(rcoords(f,3))   round(rcoords(f,3))   round(rcoords(f,3))+1 round(rcoords(f,3))-1], ...
                    2 , f) = 255;
                vidchunk([round(rcoords(f,2)) round(rcoords(f,2))+1 round(rcoords(f,2))-1 round(rcoords(f,2))   round(rcoords(f,2))  ], ...
                    [round(rcoords(f,3)) round(rcoords(f,3))   round(rcoords(f,3))   round(rcoords(f,3))+1 round(rcoords(f,3))-1], ...
                    3 , f) = 50;
            catch
                %fprintf('missed green LED at frame:%d\n',f);
                
            end
        end
        
        %green marked with magenta
        if(max(isnan(gcoords(f,:,:)))==0)
            try
                vidchunk([round(gcoords(f,2)) round(gcoords(f,2))+1 round(gcoords(f,2))-1 round(gcoords(f,2))   round(gcoords(f,2))  ], ...
                    [round(gcoords(f,3)) round(gcoords(f,3))   round(gcoords(f,3))   round(gcoords(f,3))+1 round(gcoords(f,3))-1], ...
                    1 , f) = 255;
                vidchunk([round(gcoords(f,2)) round(gcoords(f,2))+1 round(gcoords(f,2))-1 round(gcoords(f,2))   round(gcoords(f,2))  ], ...
                    [round(gcoords(f,3)) round(gcoords(f,3))   round(gcoords(f,3))   round(gcoords(f,3))+1 round(gcoords(f,3))-1], ...
                    2 , f) = 50;
                vidchunk([round(gcoords(f,2)) round(gcoords(f,2))+1 round(gcoords(f,2))-1 round(gcoords(f,2))   round(gcoords(f,2))  ], ...
                    [round(gcoords(f,3)) round(gcoords(f,3))   round(gcoords(f,3))   round(gcoords(f,3))+1 round(gcoords(f,3))-1], ...
                    3 , f) = 50;
            catch  %#ok<*CTCH>
                %fprintf('missed red LED at frame:%d\n',f);
                
            end
            
        end
        
        %absf = absolute frame number (running total)
        absf = absf + 1;
        %frame# centroidx centroidy redx redy greenx greeny bluex bluey
        coords(absf,1:9) = [startframe-1+absf, round(mean([rcoords(f,2);gcoords(f,2)])),round(mean([rcoords(f,3);gcoords(f,3)])), ...
            round(rcoords(f,2)),round(rcoords(f,3)), round(gcoords(f,2)),round(gcoords(f,3)), 0, 0];
        
    end
    
    
    
    
    
    if(show_video)
        %combine original frames and modified frames in one struct
        tmpcell = mat2cell(cat(2,vidchunk,modchunk), vidobj.Height, vidobj.Width*2, 3, ones(1,nframes));
        combinedframes = cell2struct(tmpcell, 'cdata',1);
        combinedframes(1).colormap = [];
        
        set(mymoviefig, 'position', [400 400 vidobj.Width*2 vidobj.Height])
        movie(mymoviefig, combinedframes, 1, playback_rate)
    end%end of if(show_video)
end%end of chunk loop





if(interpolate_at_end)
    fprintf('interpolating to fill in missing coordinates. . .\n');
    %interpolate missing coords
    %this time, use whole video's data
    rbadind = find(isnan(coords(:,4)) | isnan(coords(:,5)));
    rgoodind = find(~isnan(coords(:,4)) & ~isnan(coords(:,4)));
    gbadind = find(isnan(coords(:,6)) | isnan(coords(:,7)));
    ggoodind = find(~isnan(coords(:,6)) & ~isnan(coords(:,7)));
    
    %red
    coords(rbadind, 4:5) = ...      %missing red coords
        round(...                   %rounded to ints
        interp1(...                 %interpolated
        coords(rgoodind,1),...      %using known frames
        coords(rgoodind, 4:5),...   %and corresponding known coords
        coords(rbadind, 1),...      %to find the unknown frames
        'linear',...                %use linear interpolation
        'extrap'...                 %use linear extrapolation for out of range values
        ));
    
    %green
    coords(gbadind, 6:7) = ...      %missing green coords
        round(...                   %rounded to ints
        interp1(...                 %interpolated
        coords(ggoodind,1),...      %using known frames
        coords(ggoodind, 6:7),...   %and corresponding known coords
        coords(gbadind, 1),...      %to find the unknown frames
        'linear',...                %use linear interpolation
        'extrap'...                 %use linear extrapolation for out of range values
        ));
    
    %get rid of spurious, i.e., negative, interpolations
    coords(rcoords<0) = 0;
    
end%end of interpolate_at_end

if(show_plot)
    figure
    image(vidchunk(:,:,:,1))
    hold on
    %plot (y,x) instead of (x,y) due to fact that 
    %we're overlaying on pixel coordinate image
    plot(coords(:,7), coords(:,6), 'g')%green
    plot(coords(:,5), coords(:,4), 'r')%red
    plot(coords(:,3), coords(:,2), 'w')%centroid
    
end



end%end of function



