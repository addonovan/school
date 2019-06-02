function [] = d_statistics()
    data_file = 'scrambled_mnist10000.bin';
    
    best_accuracy = -Inf(1);
    best_ds = [];

    % calculate all the accuracies of each d value 
    for d = 10 : 10 : 300 
        [acc, ~] = pca_classifier_stats(data_file, d);
        if acc > best_accuracy
            best_accuracy = acc;
            best_ds = d;    
        elseif acc == best_accuracy
            best_ds = [best_ds; d];
        end
    end

    fprintf('Best accuracy:  %1.2f\n', best_accuracy);
    fprintf('Best d values:  ');
    for d = best_ds
        fprintf('%d ', d);
    end
    fprintf('\n');
end

