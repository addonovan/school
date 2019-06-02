function [digits, labels] = load_mnist(data_file)
    fid = fopen(data_file, 'r');

    [number, count] = fread(fid, 1, 'int32');
    if count ~= 1
        error('failed to read number');
    end

    [permutation, count] = fread(fid, number, 'int32');
    if count ~= permutation
        error('failed to read number');
    end


    [labels, count] = fread(fid, number, 'uchar');
    if count ~= number
        error('failed to read number');
    end
        
    digits = fread(fid, [28, 28 * number], 'uint8=>uint8');
    digits = reshape(digits, [28, 28, number]);

    fclose(fid);
end

