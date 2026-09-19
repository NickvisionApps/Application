import {invoke} from "@tauri-apps/api/core";
import {
  createContext,
  ReactNode,
  useContext,
  useEffect,
  useMemo,
  useState,
} from "react";

export type ProductInfo = {
  id: String;
  name: String;
  short_name: String;
  repo_owner: String;
  repo_name: String;
  description: String;
  version: String;
};

interface ProductInfoProviderProps {
  children: ReactNode;
}

interface ProductInfoProviderState {
  productInfo: ProductInfo;
}

const DefaultProductInfo: ProductInfo = {
  id: "",
  name: "",
  short_name: "",
  repo_owner: "",
  repo_name: "",
  description: "",
  version: "",
};

const ProductInfoProviderContext = createContext<
  ProductInfoProviderState | undefined
>(undefined);

export function ProductInfoProvider({
  children,
  ...props
}: ProductInfoProviderProps) {
  const [productInfo, setProductInfo] =
    useState<ProductInfo>(DefaultProductInfo);

  useEffect(() => {
    async function startup() {
      setProductInfo(await invoke("get_product_information"));
    }

    void startup();
  }, []);

  return (
    <ProductInfoProviderContext.Provider
      {...props}
      value={useMemo<ProductInfoProviderState>(
        () => ({
          productInfo,
        }),
        [productInfo],
      )}
    >
      {children}
    </ProductInfoProviderContext.Provider>
  );
}

export const useProductInfo = () => {
  const context = useContext(ProductInfoProviderContext);
  if (!context) {
    throw new Error("useProductInfo must be used with a ProductInfoProvider");
  }
  return context;
};
