function [eigenvectors, eigenvalues] = mnist_pca(data_file, class_label, d)
    digits = load_mnist_by_label(data_file, class_label);
    digits = vectorize_image(digits);
    digits = normalize_image(digits);
    [eigenvectors, eigenvalues] = calculate_pca(digits);

    % finally, select only the top `d` entries from both matricies
    if ~isnan(d) 
        eigenvectors = eigenvectors(:, 1 : d);
        eigenvalues = eigenvalues(1 : d);
    end

    function [eigenvectors, eigenvalues] = calculate_pca(vectors)
        % first, center all of the data 
        average = [mean(vectors')]';
        centered_vectors = vectors - average; 

        % now, find the covariance matrix
        covariance = centered_vectors * centered_vectors';
        [eigenvectors, eigenvalues] = eig(covariance);

        % sort the vectors by their corresponding eigenvalues, such that the
        % eigenvector with the highest eigenvalue are at index 1
        eigenvalues = diag(eigenvalues);
        [eigenvalues, indices] = sort(eigenvalues, 'descend');
        eigenvectors = eigenvectors(:, indices);
    end
end

