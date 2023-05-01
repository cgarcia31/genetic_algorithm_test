import pandas as pd
import matplotlib.pyplot as plt

DURATION_TEXT = 'duration(ns)'
PRODUCT_TEXT = 'len_word1 * len_word2'
MIN_TEXT = 'Min(len_word1,len_word2)'

def graph_methods_language(csvfile):
    # Load the CSV file
    df = pd.read_csv(csvfile, sep=':')

    # Iterate over the methods and create a separate graph for each method
    for method in df['method'].unique():
        # Select the rows corresponding to the current method
        df_method = df[df['method'] == method]
        
        # Create a separate scatter plot for each language
        for language in df_method['language'].unique():
            # Select the rows corresponding to the current language
            df_language = df_method[df_method['language'] == language]
           
            # Create the scatter plot
            plt.scatter(df_language['len_word1'] * df_language['len_word2'], df_language[DURATION_TEXT], label=language, s=1, color='green')
        
            # Set the title, x-axis label, and y-axis label
            plt.title(method+"-"+language)
            plt.xlabel(PRODUCT_TEXT)
            plt.ylabel(DURATION_TEXT)
        
            # Add a legend
            plt.legend()
        
            # Show the graph
            plt.savefig(method.replace(' ','_')+"-"+language+".png")
            plt.clf()

def graph_methods_min_xy(csvfile):
    # Load the CSV file
    df = pd.read_csv(csvfile, sep=':')

    # Iterate over the methods and create a separate graph for each method
    for method in df['method'].unique():
        # Select the rows corresponding to the current method
        df_method = df[df['method'] == method]
        
        # Create a separate scatter plot for each language
        for language in df_method['language'].unique():
            # Select the rows corresponding to the current language
            df_language = df_method[df_method['language'] == language]
            if language == 'C':
                color = 'green'
            else:
                color = 'red'
            # Create the scatter plot
            plt.scatter(df_language[['len_word1', 'len_word2']].min(axis=1), df_language[DURATION_TEXT], label=language, s=1, color=color)            
        
        # Set the title, x-axis label, and y-axis label
        plt.title(method+"-"+language)
        plt.xlabel(MIN_TEXT)
        plt.ylabel(DURATION_TEXT)
        # Set the x-axis to use a logarithmic scale
        # Add a legend
        plt.legend()
        
        # Save Graphs
        plt.savefig(method.replace(' ','_')+"_min_xy.png")
        plt.title(method+"-"+language+" log scale")
        plt.yscale('log')        
        plt.savefig(method.replace(' ','_')+"_min_xy_log.png")
        plt.clf()

def graph_methods_xy(csvfile):
    # Load the CSV file
    df = pd.read_csv(csvfile, sep=':')

    # Iterate over the methods and create a separate graph for each method
    for method in df['method'].unique():
        # Select the rows corresponding to the current method
        df_method = df[df['method'] == method]
        
        # Create a separate scatter plot for each language
        for language in df_method['language'].unique():
            # Select the rows corresponding to the current language
            df_language = df_method[df_method['language'] == language]
            if language == 'C':
                color = 'green'
            else:
                color = 'red'
            # Create the scatter plot
            plt.scatter(df_language['len_word1'] * df_language['len_word2'], df_language[DURATION_TEXT], label=language, s=1, color=color)
        
        # Set the title, x-axis label, and y-axis label
        plt.title(method+"-"+language)
        plt.xlabel(PRODUCT_TEXT)
        plt.ylabel(DURATION_TEXT)
        # Set the x-axis to use a logarithmic scale
        # Add a legend
        plt.legend()
        
        # Save Graphs
        plt.savefig(method.replace(' ','_')+"_xy.png")
        plt.title(method+"-"+language+" log scale")
        plt.yscale('log')        
        plt.savefig(method.replace(' ','_')+"_xy_log.png")
        plt.clf()

def show_graph(csvfile):

    # read in the csv file as a pandas dataframe
    df = pd.read_csv(csvfile, sep=':')

    # group the data by method and language, and calculate the mean duration for each group
    df = df[['method', 'language', DURATION_TEXT]].groupby(['method', 'language']).mean().reset_index()

    plt.xticks(rotation=90)

    # plot the results as a bar chart
    fig, ax = plt.subplots(figsize=(10, 6))
    ax = df.pivot(index='method', columns='language', values=DURATION_TEXT).plot(kind='bar', ax=ax)

    # Set the axis labels and title
    plt.title('Comparison of Method Duration by Language')
    plt.xlabel('Method and Language')
    plt.ylabel(DURATION_TEXT)
    plt.savefig('myplot.png')

if __name__ == "__main__":
    csv_file = 'compare_method_language4.csv'
    graph_methods_language(csv_file)
    graph_methods_xy(csv_file)
    graph_methods_min_xy(csv_file)
