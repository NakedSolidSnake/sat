#!/bin/bash 

# Função para mostrar uso
show_usage() {
    echo "Usage: $0 [target] [tag]"
    echo ""
    echo "Available targets:"
    echo "  base-deps      - Base dependencies only"
    echo "  dev-deps       - Development dependencies"
    echo "  external-libs  - External libraries compiled"
    echo "  sat-build      - SAT library compiled"
    echo "  final          - Complete image (default)"
    echo ""
    echo "Examples:"
    echo "  $0                           # Build complete image"
    echo "  $0 base-deps                 # Build only base dependencies"
    echo "  $0 dev-deps myrepo/dev:1.0   # Build dev deps with custom tag"
}

# Verificar argumentos
TARGET=${1:-final}
TAG=${2:-solidcris/development:1.0.2}

# Validar targets
case "$TARGET" in
    "base-deps"|"dev-deps"|"external-libs"|"sat-build"|"final")
        echo "Building target: $TARGET with tag: $TAG"
        docker build --target $TARGET -t $TAG .
        
        # Se for build final, criar tag latest também
        if [ "$TARGET" = "final" ] && [ "$TAG" = "solidcris/development:1.0.2" ]; then
            docker tag $TAG solidcris/development:latest
            echo "Tagged as solidcris/development:latest"
        fi
        ;;
    "help"|"--help"|"-h")
        show_usage
        ;;
    *)
        echo "Error: Unknown target '$TARGET'"
        echo ""
        show_usage
        exit 1
        ;;
esac

# docker login -u solidcris  
# docker push -a solidcris/development
