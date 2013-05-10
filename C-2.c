


for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
{
    
    free(train_features[i]);
    free(test_features[i]);
}

for(i=0;i<n1;i++)
{
    free( weight_hidden_layer[i]);
    free(change_weight_hidden_layer[i]);
}

for(i=0;i<n2;i++)
{
    free( weight_hidden_layer[i]);
    free(change_weight_output_layer[i]);
    
}


free(train_features);
free(test_features);
free(weight_hidden_layer);
free(change_weight_hidden_layer);
free( weight_hidden_layer);
free(change_weight_output_layer);
free(train_class_label);
free(test_class_label);
free(predicted_class_label);
free(diff);
free(op_hidden_layer);
free(op_result_layer);