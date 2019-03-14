function result = ssd_bn_search(image, template)
    normalize = @(img) img - mean(img(:));
    result = template_search(image, template, normalize);
end

