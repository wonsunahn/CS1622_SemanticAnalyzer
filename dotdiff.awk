#! /usr/bin/awk -f
# dotdiff.awk <encumbant.gv> <candidate.gv>
# Use to illuminate the commonality and differences between dot files.


# associate the count with an edge
function parseEdge(str, arr){
    # can't count on label being paren-free anymore
    n = match($0, / [(][[:digit:]]*[)]>];$/)
    if(n>0)
        arr[substr($0,1,n)] = substr($0, n+2, length($0)-n-5)
    else {
        # Remove semicolon from the edge key
        edge_key = $1 " " $2 " " $3
        gsub(/;$/, "", edge_key)  # Remove trailing semicolon
        arr[edge_key " "]++
    }
}

# strip metadata from filename
function de_path_ext(pth){
    split(pth, a, "/")
    return substr(a[length(a)], 1, index(a[length(a)], ".")-1)
}

BEGIN {
    NOCOUNTS=0
    DFLT="black"
    GAIN="blue"
    DROP="red"
    DIFF="orange"
}

# collect the nodes from both files
NR==FNR && /^[[:space:]]*[^ ]+ \[label=.*\] ;/ {
    # Extract node ID and label from format: nodeID [label="label text"] ;
    if(match($0, /^[[:space:]]*([^ ]+) \[label=(.+)\] ;/, parts)) {
        node1[parts[1]] = parts[2]
    }
}
NR!=FNR && /^[[:space:]]*[^ ]+ \[label=.*\] ;/ {
    # Extract node ID and label from format: nodeID [label="label text"] ;
    if(match($0, /^[[:space:]]*([^ ]+) \[label=(.+)\] ;/, parts)) {
        node2[parts[1]] = parts[2]
    }
}

# collect the edges from both files
NR==FNR && /^[[:space:]]*[^ ]+ -- [^ ]+/ {
    label1 = FILENAME;
    parseEdge($0, edge1)
}
NR!=FNR && /^[[:space:]]*[^ ]+ -- [^ ]+/ {
    label2 = FILENAME;
    parseEdge($0, edge2)
}

END{
    # Print nodes with appropriate colors:
    # - In both node1 and node2 with same label: DFLT color
    # - In both node1 and node2 with different label: DIFF color
    # - Only in node1: DROP color
    # - Only in node2: GAIN color

    print "graph {"
    print "charset=\"utf-8\";"
    print "fontname=\"Helvetica,Arial,sans-serif\" \n"
    print "node [fontname=\"Helvetica,Arial,sans-serif\"] \n"
    print "edge [fontname=\"Helvetica,Arial,sans-serif\"] \n"

    # Print nodes in both node1 and node2
    n1 = asorti(node1, sorted_nodes1)
    for(i = 1; i <= n1; i++) {
        n = sorted_nodes1[i]
        if(n in node2) {
            if(node1[n] == node2[n]) {
                # Same node ID and same label - default color
                print "    " n " [label=" node1[n] " color=\"" DFLT "\"];"
            } else {
                # Same node ID but different label - different color
                print "    " n " [label=" node1[n] " color=\"" DIFF "\" penwidth=3];"
            }
        }
    }
    # Print nodes only in node1 (DROP color)
    for(i = 1; i <= n1; i++) {
        n = sorted_nodes1[i]
        if(!(n in node2)) {
            print "    " n " [label=" node1[n] " color=\"" DROP "\" penwidth=3];"
        }
    }
    # Print nodes only in node2 (GAIN color)
    n2 = asorti(node2, sorted_nodes2)
    for(i = 1; i <= n2; i++) {
        n = sorted_nodes2[i]
        if(!(n in node1)) {
            print "    " n " [label=" node2[n] " color=\"" GAIN "\" penwidth=3];"
        }
    }

    # Edges
    for(e in edge1){
        if(!(e in edge2)) edges[e "[color=\"" DROP "\" penwidth=3];"];
        else {
            # edge is in both graphs
            edges[e "[color=\"" DFLT "\"];"]
            delete edge2[e]
        }
    }
    # only edges not in first file
    for(e in edge2)
        edges[e "[color=\"" GAIN "\" penwidth=3];"];  # expects label=<tag> to close

    n = asorti(edges,output)
    for(i=1;i<=n;i++ ){print output[i]}

    print "\n}\n"
}