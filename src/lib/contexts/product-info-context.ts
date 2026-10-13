import {createContext, useContext} from "react";

export type ProductInfo = {
  id: string;
  name: string;
  short_name: string;
  repo_owner: string;
  repo_name: string;
  description: string;
  version: string;
};

export interface ProductInfoProviderState {
  productInfo: ProductInfo;
}

export const ProductInfoProviderContext = createContext<
  ProductInfoProviderState | undefined
>(undefined);

export const useProductInfo = () => {
  const context = useContext(ProductInfoProviderContext);
  if (!context) {
    throw new Error("useProductInfo must be used with a ProductInfoProvider");
  }
  return context;
};
