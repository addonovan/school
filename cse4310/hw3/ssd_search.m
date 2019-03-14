function result = ssd_search(image, template)
    noop = @(img) img;
    result = template_search(image, template, noop);
end


